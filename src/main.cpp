//#include "ManualSetupPayloadParser.h"
#include "QRCodeSetupPayloadParser.h"
#include "SetupPayload.h"
#include <string>
#include <vector>
#include <fmt/core.h>
#include <fmt/color.h>
#include <iostream>

namespace {

} // namespace

bool IsQRCode(std::string codeString)
{
    return codeString.rfind(chip::kQRCodePrefix) == 0;
}

int Parse(std::string codeString, chip::SetupPayload & payload)
{
    bool isQRCode = IsQRCode(codeString);
    
    enum class color { red, green, blue };
    auto s = fmt::format("{}", fmt::underlying(color::red));


    std::string str = isQRCode ? "base38" : "decimal";
    
    fmt::print(fmt::fg(fmt::color::alice_blue), "Parsing '{}'. Representation: '{}'\n", str, codeString);
    return chip::QRCodeSetupPayloadParser(codeString).populatePayload(payload);
}


void PrintSetup(const chip::SetupPayload& payload) {
    
    fmt::println("--------------");
    
    fmt::print("Version:  {}\n", payload.version);
    fmt::print("VendorID: {}\n", payload.vendorID);
    fmt::print("ProductID:  {}\n", payload.productID);
    fmt::print("Custom flow:  {}\n", std::to_string(static_cast<uint8_t>(payload.commissioningFlow)).c_str());
    
    if (payload.rendezvousInformation.has_value())
    {
        //fmt::println("RendezvousInformation: {}", std::to_string(payload.rendezvousInformation.value().Raw()).c_str());
        if (payload.rendezvousInformation.value().Has(chip::RendezvousInformationFlag::kOnNetwork)) {
            fmt::println("rendezvous: 'OnTheNetwork'");
        }
        if (payload.rendezvousInformation.value().Has(chip::RendezvousInformationFlag::kBLE)) {
            fmt::println("rendezvous: 'BLE'");
        }
        if (payload.rendezvousInformation.value().Has(chip::RendezvousInformationFlag::kSoftAP)) {
            fmt::println("rendezvous: 'SoftAP'");
        }
    }
    if (payload.discriminator.IsShortDiscriminator())
    {
        fmt::println("Short discriminator: {}", std::to_string(payload.discriminator.GetShortValue()).c_str());
    }
    else
    {
        fmt::println("Long discriminator: {}", std::to_string(payload.discriminator.GetLongValue()).c_str());
    }
    
    fmt::println("SetUpPINCode: {}", std::to_string(payload.setUpPINCode).c_str());

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

int main()
{
    const char* mCode = "MT:Y.K9042C00KA0648G00";

    std::string codeString(mCode);
    chip::SetupPayload payload;

    int err = Parse(codeString, payload);
    if (err == chip::CHIP_NO_ERROR) {
        PrintSetup(payload);
    } else {
        fmt::print(fmt::fg(fmt::color::rebecca_purple), "QR code parsing error: {}\n", err);
    }

    return 0;
}


