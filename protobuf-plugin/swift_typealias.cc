#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/compiler/plugin.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>

using namespace google::protobuf;

/// Generates Swift typealiases for Protobuf types.
class Generator : public  compiler::CodeGenerator {
    std::string GetOutputFilename(const std::string& proto_file) const {
        int index = proto_file.find_last_of(".");
        return proto_file.substr(0, index) + "+Proto.swift";
    }

    bool Generate(const FileDescriptor* file, const std::string& parameter, compiler::GeneratorContext* generator_context, string* error) const {
        std::unique_ptr<io::ZeroCopyOutputStream> output(generator_context->Open(GetOutputFilename(file->name())));
        io::Printer printer(output.get(), '$');

        printer.Print(
            "// Copyright © 2017-2019 Trust Wallet.\n"
            "//\n"
            "// This file is part of Trust. The full Trust copyright notice, including\n"
            "// terms governing use, modification, and redistribution, is contained in the\n"
            "// file LICENSE at the root of the source code distribution tree.\n"
            "\n"
        );
        for (int i = 0; i < file->message_type_count(); i += 1) {
            auto message = file->message_type(i);
            auto parts = Generator::getParts(message->full_name());
            if (parts.size() < 3 || parts[0] != "TW") {
                std::cerr << "Invalid proto name '" << message->full_name() << "'" << std::endl;
                continue;
            }
            std::string def = "public typealias ";
            for (auto i = 0; i < parts.size(); i += 1) {
                if (i == 0 || i == 2) {
                    continue;
                }
                def += parts[i];
            }

            def += " = ";

            for (auto& part : parts) {
                def += part + "_";
            }
            def = def.substr(0, def.size() - 1);
            def += ";\n";
            printer.Print(def.c_str());
        }

        return true;
    }

    static std::vector<std::string> getParts(const std::string& string) {
        size_t curr = 0;
        size_t pos = 0;
        std::vector<std::string> parts;

        while (curr < string.size() && (pos = string.find('.', curr)) != std::string::npos) {
            const auto part = string.substr(curr, pos - curr);
            parts.push_back(part);
            curr = pos + 1;
        }

        if (curr < string.size())
            parts.push_back(string.substr(curr));

        return parts;
    }
};

int main(int argc, char* argv[]) {
  Generator generator;
  return compiler::PluginMain(argc, argv, &generator);
}
