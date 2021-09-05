//
// Created by York on 2021/4/10.
//

#include <ryml_std.hpp>
#include <ryml.hpp>
#include <glog/logging.h>
#include <fmt/format.h>

#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>

#include "flr-core/flr.h"

namespace fs = std::__fs::filesystem;

void flr_opt_log_callback(std::string log_msg) {
    LOG(INFO) << log_msg;
}

void test_glog_api() {
    LOG(INFO)  << "Hello, World! Hello C++! !" << std::endl;
}

void test_ryml_api() {
    // ryml can parse in situ (and read-only buffers too):
    char src[] = "{foo: 1, bar: [2, 3], map: {A: a}}";
    c4::substr srcview = src; // a mutable view to the source buffer
    // there are also overloads for reusing the tree and parser
    ryml::Tree tree = ryml::parse(srcview);
    ryml::NodeRef root = tree.rootref();
    root |= ryml::MAP; // mark root as a map

    // get a reference to the "foo" node
    if (tree.rootref().has_child("foo")) {
        LOG(INFO) << "XX= " << tree["foo"].val();
    }
    ryml::NodeRef node = tree["foo"];
    LOG(INFO) << node; // "foo: 1"
    LOG(INFO) << tree["bar"][0]; // "2"
    LOG(INFO) << tree["bar"][1]; // "3"

    // deserializing:
    int foo;
    std::cout << "foo(deserializing before): " << foo << std::endl;
    node >> foo; // now foo == 1
    std::cout << "foo(deserializing after): " << foo << std::endl;

    ryml::NodeRef map = root["map"];
    map |= ryml::MAP;
    map["A"] << "A++";
    map["Age"] << 5;
    std::vector<std::string> array;
    array.push_back("assets");
    array.push_back("fonts");
    map["array"] << array;

    ryml::NodeRef xmas5 = root["xmasFifthDay"];
    xmas5 |= ryml::MAP;
    xmas5["callingBirds"] = "four";
    xmas5["frenchHens"] << 3;
    xmas5["goldenRings"] << 5;

    std::cout << "new tree:\n" << tree << std::endl;
}

void test_fmt_api() {
    std::string flutter_sdk_mmp_version = "1.12.13+hotfix.5";
    std::size_t hotfix_found = flutter_sdk_mmp_version.find("+");
    if (hotfix_found != std::string::npos) {
        flutter_sdk_mmp_version.erase(hotfix_found);
    }
    LOG(INFO) << flutter_sdk_mmp_version;

    std::string asset_name = "assets/images/t$est$.png";
    std::string escaped_asset_name = regex_replace(asset_name, std::regex("[$]"), "\\$");
    LOG(INFO) << escaped_asset_name;

    std::string code = R"CODE(
  /// {}
  // ignore: non_constant_identifier_names
  final {} = const AssetResource("{}", packageName: R.package);
)CODE";
    code.erase(0, 1);
    code = fmt::format(code, "asset_comment", "asset_id", escaped_asset_name);

    LOG(INFO) << code;

    // 转义序列 {{ 与 }} ，它们在输出中被分别替换成 { 与 }
    code = fmt::format(R"CODE(
// ignore: camel_case_types
class _R_Svg_AssetResource {{
  const _R_Svg_AssetResource();
{0}
{0}
{0}
}}
)CODE",
                       "  func f();");

    // 移除 "\n" 首字符
    code.erase(0, 1);
    LOG(INFO) << code;
}

void test_stringstream_api() {
    std::stringstream warning_messages;
    warning_messages << "hello" << std::endl;
    warning_messages << "stringstream" << std::endl;
    std::string fon_family_name = fs::path("/flutter_hello_app/fonts/Song").stem();
    LOG(INFO) << warning_messages.str() << fon_family_name << std::endl;
}

void test_flr_api() {
    LOG(INFO) << "========== test flr::init_one ==========";
    flr::init_one("/Users/York/Workspace/Flutterspace/flutter_hello_app", "2.0.3", "2.12.2", flr_opt_log_callback);

    LOG(INFO) << "========== test flr::generate_one ==========";
    flr::generate_one("/Users/York/Workspace/Flutterspace/flutter_hello_app", "2.0.3", "2.12.2", flr_opt_log_callback);
}

int main() {
    test_flr_api();
    return 0;
}