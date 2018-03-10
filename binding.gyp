{
    "targets": [
    {
        "include_dirs": [
            "<!(node -e \"require('nan')\")"
        ],
        "target_name": "yaz0",
        "sources": [
            "src/main.cpp",
                "src/yaz0_base.cpp",
                "src/yaz0.cpp",
                "src/yaz0_creator.cpp"
        ],
        "cflags": [
          "-std=c++11",
          '-O3'
        ]
    }
    ]
}
