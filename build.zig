const std = @import("std");

const gdstk_sources = [_][]const u8{
    // GDSTK
    "src/cell.cpp",
    "src/clipper_tools.cpp",
    "src/curve.cpp",
    "src/flexpath.cpp",
    "src/gdsii.cpp",
    "src/label.cpp",
    "src/library.cpp",
    "src/oasis.cpp",
    "src/polygon.cpp",
    "src/property.cpp",
    "src/rawcell.cpp",
    "src/reference.cpp",
    "src/repetition.cpp",
    "src/robustpath.cpp",
    "src/style.cpp",
    "src/utils.cpp",

    // GDSTK c wrappers
    "c/cell.cpp",
    "c/curve.cpp",
    "c/label.cpp",
    "c/library.cpp",
    "c/polygon.cpp",
    "c/reference.cpp",
    "c/utils.cpp",
};

pub fn build(b: *std.Build) void {
    const opts = .{
        .target = b.standardTargetOptions(.{}),
        .optimize = b.standardOptimizeOption(.{}),
    };
    // const link_mode = b.option(
    //     std.builtin.LinkMode,
    //     "linkage",
    //     "how the library should be linked (default: static)",
    // );

    const zlib_dep = b.dependency("zlib", opts);
    const qhull_dep = b.dependency("qhull", opts);

    const libgdstk = b.addLibrary(.{
        .name = "gdstk",
        .linkage = .static,
        .root_module = b.createModule(.{
            .target = opts.target,
            .optimize = opts.optimize,
            .link_libc = true,
            .link_libcpp = true,
            .pic = true,
        }),
    });
    libgdstk.addCSourceFiles(.{
        .root = b.path(""),
        .files = &gdstk_sources,
        // .flags = &.{ "" },
    });

    libgdstk.root_module.addIncludePath(b.path("include"));
    libgdstk.root_module.addIncludePath(b.path("c"));
    libgdstk.root_module.addIncludePath(b.path("external"));

    libgdstk.root_module.linkSystemLibrary("m", .{});
    libgdstk.root_module.linkLibrary(qhull_dep.artifact("qhull"));
    libgdstk.root_module.linkLibrary(zlib_dep.artifact("z"));

    libgdstk.installHeadersDirectory(b.path("c"), "", .{
        // .include_extensions = &.{ ".h", ".hpp" },
    });
    b.installArtifact(libgdstk);

    // translate-c the libraw.h file
    const translate_c = b.addTranslateC(.{
        .root_source_file = b.path("c/gdstk.h"),
        .target = opts.target,
        .optimize = opts.optimize,
    });

    // ...and the Zig module for the generated bindings
    const mod = b.addModule("gdstk", .{
        .root_source_file = translate_c.getOutput(),
        .target = opts.target,
        .optimize = opts.optimize,
        .link_libc = true,
        .link_libcpp = true,
    });
    mod.linkLibrary(libgdstk);

    // =================================
    // TESTS
    // =================================

    const test_step = b.step("test", "Run tests");
    const unit_tests = b.addTest(.{
        .name = "tests",
        .root_module = b.createModule(.{
            .root_source_file = b.path("tests/zig/root.zig"),
            .target = opts.target,
            .optimize = opts.optimize,
            .imports = &.{
                .{ .name = "gdstk", .module = mod },
            },
        }),
    });
    unit_tests.root_module.linkLibrary(libgdstk);
    const run_unit_tests = b.addRunArtifact(unit_tests);
    test_step.dependOn(&run_unit_tests.step);
}
