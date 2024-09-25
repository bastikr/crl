add_library(crl
    "src/crl.cpp"
    "src/crlv.cpp"
    "src/utils_strv.cpp"
)

target_include_directories(crl PUBLIC include)
