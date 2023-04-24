#include "interface.h"
#include "CHIPError.h"
#include "QRCodeSetupPayloadParser.h"
#include "SetupPayload.h"
#include <fmt/color.h>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>

// Initialize.
Event_t event = Event_t{
    .cb = 0,
    .context = 0,
};

bool IsQRCode(std::string codeString)
{
    return codeString.rfind(chip::kQRCodePrefix) == 0;
}

void PrintSetup(const chip::SetupPayload& payload, QSetup_t* out) {
    
    fmt::println("--------------");
    
    out->Version = payload.version;
    out->VendorID = payload.vendorID;
    out->ProductID = payload.productID;

    fmt::print("Version:  {}\n", payload.version);
    fmt::print("VendorID: {}\n", payload.vendorID);
    fmt::print("ProductID:  {}\n", payload.productID);
    fmt::print("Custom flow:  {}\n", std::to_string(static_cast<uint8_t>(payload.commissioningFlow)).c_str());
    
    if (payload.rendezvousInformation.has_value())
    {
        //fmt::println("RendezvousInformation: {}", std::to_string(payload.rendezvousInformation.value().Raw()).c_str());
        if (payload.rendezvousInformation.value().Has(chip::RendezvousInformationFlag::kOnNetwork)) {
            fmt::println("rendezvous: 'OnTheNetwork'");
            out->Rendezvous = EnumRendezvous::ONNETWORK;
        }
        if (payload.rendezvousInformation.value().Has(chip::RendezvousInformationFlag::kBLE)) {
            fmt::println("rendezvous: 'BLE'");
            out->Rendezvous = EnumRendezvous::BLE;
        }
        if (payload.rendezvousInformation.value().Has(chip::RendezvousInformationFlag::kSoftAP)) {
            fmt::println("rendezvous: 'SoftAP'");
            out->Rendezvous = EnumRendezvous::SOFTAP;
        }
    }
    if (payload.discriminator.IsShortDiscriminator())
    {
        fmt::println("Short discriminator: {}", std::to_string(payload.discriminator.GetShortValue()).c_str());
        out->Discriminator = payload.discriminator.GetShortValue();
    }
    else
    {
        fmt::println("Long discriminator: {}", std::to_string(payload.discriminator.GetLongValue()).c_str());
        out->Discriminator = payload.discriminator.GetLongValue();
    }
    
    fmt::println("SetUpPINCode: {}", std::to_string(payload.setUpPINCode).c_str());
    out->Passcode = payload.setUpPINCode;

    std::string serialNumber;
    if (payload.getSerialNumber(serialNumber) == chip::CHIP_NO_ERROR)
        fmt::println("SerialNumber", serialNumber.c_str());

    for (const chip::OptionalQRCodeInfo & info : payload.getAllOptionalVendorData())
    {
        if (info.type == chip::optionalQRCodeInfoTypeString)
            fmt::println("Optional {}: {}", info.tag, info.data.c_str());

        if (info.type == chip::optionalQRCodeInfoTypeInt32)
            fmt::println("Optional2 {}: {}", info.tag, std::to_string(info.int32).c_str());
    }

    fmt::println("--------------");
}

int QRParse(const char* in, QSetup_t *out)
{
    if (out == 0) {
        fmt::print(fmt::fg(fmt::color::red), "Invalid argument input QSetup variable can not be nil.\n");
        return chip::CHIP_ERROR_INVALID_ARGUMENT;
    }

    std::string codeString = std::string{in};
    fmt::print(fmt::fg(fmt::color::alice_blue), "Representation: '{}'\n", codeString);

    bool isQRCode = IsQRCode(codeString);
    if (!isQRCode) {
        fmt::print(fmt::fg(fmt::color::yellow), "Only base38 encoded QR code is implemented.\n");
        return chip::CHIP_ERROR_NOT_IMPLEMENTED;
    }

    chip::SetupPayload payload;
    int err_code = chip::QRCodeSetupPayloadParser(codeString).populatePayload(payload);
    PrintSetup(payload, out);
    return err_code;
}


int Run(const char* in) {

    // Callback     
    if (event.cb == 0) {
        fmt::print(fmt::fg(fmt::color::yellow), "Register callback function first.\n");
        return chip::CHIP_ERROR_INVALID_ARGUMENT;
    }

    auto setup = QSetup_t{
        .Version = 0,
        .VendorID = 0,
        .ProductID = 0,
        .Rendezvous = NONE,
        .Discriminator = 0,
        .Passcode = 0,
    };

    int ret = QRParse(in, &setup);
    if (ret != 0) {
        return ret;
    }

    event.cb(&setup, event.context);
    return 0;
}

void RegisterCallback(callback cb, void* context) {
    
    // assign
    event.cb = cb;
    event.context = context; 
}
