const std = @import("std");

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const exe = b.addExecutable(.{
        .name = "parser_cli",
        .root_source_file = .{ .path = "./src/main.zig" },
        .optimize = optimize,
    });
    b.default_step.dependOn(&exe.step);

    exe.addIncludePath("../../include");
    exe.addIncludePath("../../third_party/fmt/include");
    exe.addIncludePath("../../third_party/nlio/include");
    exe.addIncludePath("../../third_party/nlassert/include");

    exe.addCSourceFiles(&.{
        "../../src/interface.cpp",
        "../../src/Base38Decode.cpp",
        "../../src/BytesToHex.cpp",
        "../../src/CHIPMem-Malloc.cpp",
        "../../src/QRCodeSetupPayloadParser.cpp",
        "../../src/SetupPayload.cpp",
        "../../src/TLVReader.cpp",
    }, &.{
        "-std=c++17",
        "-Wall",
        "-W",
        "-DFMT_HEADER_ONLY",
    });

    exe.linkLibC();
    exe.linkLibCpp();
    b.installArtifact(exe);
}
