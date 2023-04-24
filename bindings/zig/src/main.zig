const std = @import("std");

const face = @cImport(
    @cInclude("interface.h"),
);

const Foo = struct {
    pub fn callback(self: Foo, setup: *face.QSetup_t) void {
        _ = self;
        std.debug.print("MyTest callback, Setup: {}\n", .{
            setup,
        });
    }
};

// Gateway function
pub fn callback(setup: *face.QSetup_t, context: *anyopaque) void {
    var foo_ptr: *Foo = @ptrCast(*Foo, context);
    foo_ptr.callback(setup);
}

pub fn main() void {
    //
    const argv = std.os.argv;
    if (argv.len == 1) {
        std.debug.print("Provide QR code as string argument\n", .{});
        return;
    }

    std.debug.print("Hello from Zig!\n", .{});

    // method 1.
    var foo = Foo{};
    face.RegisterCallback(@ptrCast(face.callback, &callback), &foo);
    const ret = face.Run(argv[1]);
    if (ret != 0) {
        std.debug.print("Parsing failed with error: {d}", .{ret});
    }

    // method 2.
    var setup = face.QSetup_t{
        .VendorID = 0,
        .ProductID = 0,
        .Discriminator = 0,
        .Passcode = 0,
        .Version = 0,
        .Rendezvous = 0,
    };

    // Pass argument as input string (QR string)
    const ret2 = face.QRParse(argv[1], &setup);
    if (ret2 == 0) {} else {
        std.debug.print("Parsing failed with error: {d}", .{ret2});
    }
}
