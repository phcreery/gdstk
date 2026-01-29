// GDSTK Curve Test Suite in Zig
//
// This file is a Zig translation of tests/curve_test.py and test/curve_test.c,
// demonstrating the Zig bindings for the GDSTK library. It tests curve operations
// and point management.
//
// Copyright 2020 Lucas Heitzmann Gabrielli.
// This file is part of gdstk, distributed under the terms of the
// Boost Software License - Version 1.0.  See the accompanying
// LICENSE file or <http://www.boost.org/LICENSE_1_0.txt>

const std = @import("std");
const testing = std.testing;
const expect = testing.expect;
const expectEqual = testing.expectEqual;
const expectApproxEqRel = testing.expectApproxEqRel;

// GDSTK C wrapper bindings (assuming this exists)
// const gdstk = @cImport({
//     @cInclude("gdstk.h");
// });
const gdstk = @import("gdstk");

// Helper structures matching C wrapper
const Vec2 = extern struct {
    x: f64,
    y: f64,
    pub fn toGDSTK(self: Vec2) gdstk.Vec2 {
        return gdstk.Vec2{ .unnamed_0 = .{ .unnamed_0 = .{
            .x = self.x,
            .y = self.y,
        } } };
    }
    pub fn fromGDSTK(v: gdstk.Vec2) Vec2 {
        return Vec2{
            .x = v.unnamed_0.unnamed_0.x,
            .y = v.unnamed_0.unnamed_0.y,
        };
    }
};

// const Array = extern struct {
//     capacity: u64,
//     count: u64,
//     items: ?*anyopaque,
// };

// Helper functions
fn vec2Equal(a: Vec2, b: Vec2, tolerance: f64) bool {
    return (@abs(a.x - b.x) <= tolerance) and (@abs(a.y - b.y) <= tolerance);
}

fn vec2ArraysEqual(arr1: []const Vec2, arr2: []const Vec2, tolerance: f64) bool {
    if (arr1.len != arr2.len) return false;

    for (arr1, arr2) |a, b| {
        if (!vec2Equal(a, b, tolerance)) return false;
    }
    return true;
}

// Test 1: Curve initialization (matches test_init() in Python)
test "curve initialization" {
    // Test 1: Create curve with complex initial point (1j = 0 + 1i)
    const initial_pos1 = Vec2{ .x = 0.0, .y = 1.0 };
    const curve1 = gdstk.curve_new(initial_pos1.toGDSTK(), 1e-2);
    defer gdstk.curve_free(curve1);

    try expect(curve1 != null);

    // Check tolerance
    const tolerance1 = gdstk.curve_get_tolerance(curve1);
    try expectApproxEqRel(@as(f64, 1e-2), tolerance1, 1e-10);

    // Set new tolerance
    gdstk.curve_set_tolerance(curve1, 1e-1);
    const new_tolerance1 = gdstk.curve_get_tolerance(curve1);
    try expectApproxEqRel(@as(f64, 1e-1), new_tolerance1, 1e-10);

    // Check points - should have [[0, 1]]
    const point_count1 = gdstk.curve_point_count(curve1);
    try expectEqual(@as(u64, 1), point_count1);

    const first_point = gdstk.curve_first_point(curve1);
    try expectApproxEqRel(@as(f64, 0.0), Vec2.fromGDSTK(first_point).x, 1e-6);
    try expectApproxEqRel(@as(f64, 1.0), Vec2.fromGDSTK(first_point).y, 1e-6);

    // Test 2: Create curve with different initial point and tolerance - (4, 5), 1e-3
    const initial_pos2 = Vec2{ .x = 4.0, .y = 5.0 };
    const curve2 = gdstk.curve_new(initial_pos2.toGDSTK(), 1e-3);
    defer gdstk.curve_free(curve2);

    try expect(curve2 != null);

    const tolerance2 = gdstk.curve_get_tolerance(curve2);
    try expectApproxEqRel(@as(f64, 1e-3), tolerance2, 1e-10);

    // Check points - should have [[4, 5]]
    const point_count2 = gdstk.curve_point_count(curve2);
    try expectEqual(@as(u64, 1), point_count2);

    const first_point2 = Vec2.fromGDSTK(gdstk.curve_first_point(curve2));
    try expectApproxEqRel(@as(f64, 4.0), first_point2.x, 1e-6);
    try expectApproxEqRel(@as(f64, 5.0), first_point2.y, 1e-6);
}

// Test 2: Curve point management (matches test_points() in Python)
test "curve points management" {
    // Test first case: points = [(0, 1), (1, 0), (-1, -1)]
    const initial_pos = Vec2{ .x = 0.0, .y = 1.0 };
    const curve = gdstk.curve_new(initial_pos.toGDSTK(), 1e-2);
    defer gdstk.curve_free(curve);

    // Add segment to (1, 0) - equivalent to curve.segment(points[1]) in Python
    const point = Vec2{ .x = 1.0, .y = 0.0 };
    gdstk.curve_append(curve, point.toGDSTK());

    var point_count = gdstk.curve_point_count(curve);
    try expectEqual(@as(u64, 2), point_count);

    // Check points match [(0, 1), (1, 0)]
    const expected_points1 = [_]Vec2{
        Vec2{ .x = 0.0, .y = 1.0 },
        Vec2{ .x = 1.0, .y = 0.0 },
    };
    const actual_point0 = gdstk.curve_get_point(curve, 0);
    const actual_point1 = gdstk.curve_get_point(curve, 1);
    const actual_points1 = [_]Vec2{
        Vec2.fromGDSTK(actual_point0),
        Vec2.fromGDSTK(actual_point1),
    };

    try expect(vec2ArraysEqual(&actual_points1, &expected_points1, 1e-6));

    // Add another segment to (-1, -1) - equivalent to curve.segment(points[2]) in Python
    gdstk.curve_append(curve, Vec2.toGDSTK(Vec2{ .x = -1.0, .y = -1.0 }));

    point_count = gdstk.curve_point_count(curve);
    try expectEqual(@as(u64, 3), point_count);

    // Check all points match [(0, 1), (1, 0), (-1, -1)]
    const expected_points2 = [_]Vec2{
        Vec2{ .x = 0.0, .y = 1.0 },
        Vec2{ .x = 1.0, .y = 0.0 },
        Vec2{ .x = -1.0, .y = -1.0 },
    };
    const actual_point2 = gdstk.curve_get_point(curve, 2);
    const actual_points2 = [_]Vec2{
        Vec2.fromGDSTK(actual_point0),
        Vec2.fromGDSTK(actual_point1),
        Vec2.fromGDSTK(actual_point2),
    };

    try expect(vec2ArraysEqual(&actual_points2, &expected_points2, 1e-6));
}

// Test 3: Curve segment array (matches second part of test_points() in Python)
test "curve segment array with tolerance" {
    // Test second case: points = [(0, 1), (1, 0), (1, 0), (-1, -1), (0.02, 1.02)]
    // Note: C++ segment() doesn't filter points based on tolerance (unlike Python wrapper)
    const initial_pos2 = Vec2{ .x = 0.0, .y = 1.0 };
    const curve2 = gdstk.curve_new(initial_pos2.toGDSTK(), 1e-1); // Higher tolerance
    defer gdstk.curve_free(curve2);

    // Create array of segment points: (1, 0), (1, 0), (-1, -1), (0.02, 1.02)
    // This matches the Python: curve.segment(points[1:])
    var segment_points = [_]gdstk.Vec2{
        Vec2.toGDSTK(Vec2{ .x = 1.0, .y = 0.0 }),
        Vec2.toGDSTK(Vec2{ .x = 1.0, .y = 0.0 }),
        Vec2.toGDSTK(Vec2{ .x = -1.0, .y = -1.0 }),
        Vec2.toGDSTK(Vec2{ .x = 0.02, .y = 1.02 }),
    };

    // Create Array structure for the C wrapper
    var point_array = gdstk.Array{
        .capacity = 4,
        .count = 4,
        .items = @ptrCast(&segment_points),
    };

    // Use segment_array which should handle the multiple points
    gdstk.curve_segment_array(curve2, &point_array, false);

    const point_count = gdstk.curve_point_count(curve2);

    // Check if curve is closed with tolerance
    const is_closed = gdstk.curve_is_closed(curve2);

    // The C++ segment() function does not filter points based on tolerance.
    // All 5 points should be present: [(0, 1), (1, 0), (1, 0), (-1, -1), (0.02, 1.02)]
    // The curve will be detected as "closed" due to tolerance, but the points remain.
    try expectEqual(@as(u64, 5), point_count); // Should have 5 points (segment() doesn't filter)
    try expect(is_closed); // Curve should be detected as closed due to tolerance

    // Verify the expected points (all 5 points should be present)
    const expected_points3 = [_]Vec2{
        Vec2{ .x = 0.0, .y = 1.0 },
        Vec2{ .x = 1.0, .y = 0.0 },
        Vec2{ .x = 1.0, .y = 0.0 },
        Vec2{ .x = -1.0, .y = -1.0 },
        Vec2{ .x = 0.02, .y = 1.02 },
    };

    var actual_points3: [5]Vec2 = undefined;
    for (0..5) |i| {
        actual_points3[i] = Vec2.fromGDSTK(gdstk.curve_get_point(curve2, @intCast(i)));
    }

    try expect(vec2ArraysEqual(&actual_points3, &expected_points3, 1e-6));
}

// Test 4: Curve operations (additional tests beyond Python)
test "curve operations" {
    const initial_pos = Vec2{ .x = 0.0, .y = 0.0 };
    const curve = gdstk.curve_new(initial_pos.toGDSTK(), 1e-2);
    defer gdstk.curve_free(curve);

    // Test horizontal line
    gdstk.curve_horizontal(curve, 5.0, false);

    var point_count = gdstk.curve_point_count(curve);
    try expectEqual(@as(u64, 2), point_count);

    const last_point = Vec2.fromGDSTK(gdstk.curve_last_point(curve));
    try expectApproxEqRel(@as(f64, 5.0), last_point.x, 1e-6);
    try expectApproxEqRel(@as(f64, 0.0), last_point.y, 1e-6);

    // Test vertical line
    gdstk.curve_vertical(curve, 3.0, false);

    point_count = gdstk.curve_point_count(curve);
    try expectEqual(@as(u64, 3), point_count);

    const last_point2 = Vec2.fromGDSTK(gdstk.curve_last_point(curve));
    try expectApproxEqRel(@as(f64, 5.0), last_point2.x, 1e-6);
    try expectApproxEqRel(@as(f64, 3.0), last_point2.y, 1e-6);

    // Test arc
    gdstk.curve_arc(curve, 2.0, 0.0, std.math.pi / 2.0, 0.0, 0.0);

    point_count = gdstk.curve_point_count(curve);
    try expect(point_count > 3); // Arc should add multiple points
}

// Test 5: Curve properties
test "curve properties" {
    const initial_pos = Vec2{ .x = 0.0, .y = 0.0 };
    const curve = gdstk.curve_new(initial_pos.toGDSTK(), 1e-3);
    defer gdstk.curve_free(curve);

    // Create a simple square path
    gdstk.curve_horizontal(curve, 1.0, false);
    gdstk.curve_vertical(curve, 1.0, false);
    gdstk.curve_horizontal(curve, 0.0, false);
    gdstk.curve_vertical(curve, 0.0, false);
}
