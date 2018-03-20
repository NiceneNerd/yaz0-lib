{
    "targets": [
    {
        
        "target_name": "yaz0",
        "sources": [
            "src/main.cpp",
                "src/yaz0/worker/copy_search.cpp",
                "src/yaz0/chunk.cpp",
                "src/yaz0/base.cpp",
                "src/yaz0/parser.cpp",
                "src/yaz0/creator.cpp"
        ],
        "cflags": [
          "-std=c++11",
          '-O3'
        ],
        'cflags_cc': [ '-fexceptions' ]
    }
    ]
}
