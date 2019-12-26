 static void BM_search_vector(benchmark::State& state) {
    std::random_device
        rd;    // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(
        rd());    // Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(1, 256);

    std::vector<uint8_t > v = {
        232, 194, 5,   73,  238, 115, 216, 194, 140, 192, 28,  156, 215, 83,
        253, 90,  14,  160, 95,  178, 141, 177, 114, 44,  196, 199, 150, 145,
        151, 69,  98,  168, 119, 66,  137, 193, 134, 87,  122, 62,  231, 189,
        20,  249, 245, 146, 219, 78,  38,  51,  221, 94,  30,  74,  99,  38,
        102, 86,  19,  203, 252, 66,  121, 205, 203, 35,  204, 245, 176, 41,
        53,  40,  90,  94,  242, 114, 207, 28,  198, 254, 48,  111, 85,  108,
        139, 12,  175, 181, 114, 53,  200, 217, 226, 70,  46,  243, 199, 176,
        192, 93,  26,  124, 120, 128, 191, 36,  163, 213, 247, 212, 153, 166,
        216, 45,  206, 243, 120, 29,  88,  90,  115, 168, 69,  45,  238, 220,
        131, 211, 10,  171, 17,  7,   161, 117, 51,  160, 41,  154, 183, 211,
        164, 3,   148, 5,   227, 215, 209, 220, 167, 124, 255, 56,  177, 217,
        220, 37,  224, 130, 252, 72,  62,  236, 70,  4,   152, 144, 254, 216,
        78,  203, 215, 46,  235, 27,  7,   43,  182, 87,  42,  11,  239, 222,
        117, 97,  158, 191, 173, 135, 221, 215, 41,  25,  186, 240, 38,  42,
        109, 97,  30,  255, 181, 67,  104, 233, 71,  182, 193, 122, 47,  237,
        76,  228, 202, 190, 57,  39,  181, 6,   177, 237, 241, 250, 7,   132,
        103, 120, 12,  169, 214, 206, 214, 116, 233, 114, 94,  194, 6,   197,
        150, 59,  223, 48,  46,  109, 159, 22,  36,  160, 27,  113, 30,  42,
        44,  238, 28,  155};

    bool flag = false;
    for (auto _ : state) {
        state.PauseTiming();
        auto val = dis(gen);
        state.ResumeTiming();
        benchmark::DoNotOptimize(flag = v.end() ==
                                        std::find(v.begin(), v.end(), val));
    }
    //    std::cout << flag << "\n";
}

 static void BM_search_map(benchmark::State& state) {
    std::random_device
        rd;    // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(
        rd());    // Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(1, 256);

    std::set<uint8_t> s = {
        232, 194, 5,   73,  238, 115, 216, 194, 140, 192, 28,  156, 215, 83,
        253, 90,  14,  160, 95,  178, 141, 177, 114, 44,  196, 199, 150, 145,
        151, 69,  98,  168, 119, 66,  137, 193, 134, 87,  122, 62,  231, 189,
        20,  249, 245, 146, 219, 78,  38,  51,  221, 94,  30,  74,  99,  38,
        102, 86,  19,  203, 252, 66,  121, 205, 203, 35,  204, 245, 176, 41,
        53,  40,  90,  94,  242, 114, 207, 28,  198, 254, 48,  111, 85,  108,
        139, 12,  175, 181, 114, 53,  200, 217, 226, 70,  46,  243, 199, 176,
        192, 93,  26,  124, 120, 128, 191, 36,  163, 213, 247, 212, 153, 166,
        216, 45,  206, 243, 120, 29,  88,  90,  115, 168, 69,  45,  238, 220,
        131, 211, 10,  171, 17,  7,   161, 117, 51,  160, 41,  154, 183, 211,
        164, 3,   148, 5,   227, 215, 209, 220, 167, 124, 255, 56,  177, 217,
        220, 37,  224, 130, 252, 72,  62,  236, 70,  4,   152, 144, 254, 216,
        78,  203, 215, 46,  235, 27,  7,   43,  182, 87,  42,  11,  239, 222,
        117, 97,  158, 191, 173, 135, 221, 215, 41,  25,  186, 240, 38,  42,
        109, 97,  30,  255, 181, 67,  104, 233, 71,  182, 193, 122, 47,  237,
        76,  228, 202, 190, 57,  39,  181, 6,   177, 237, 241, 250, 7,   132,
        103, 120, 12,  169, 214, 206, 214, 116, 233, 114, 94,  194, 6,   197,
        150, 59,  223, 48,  46,  109, 159, 22,  36,  160, 27,  113, 30,  42,
        44,  238, 28,  155};

    bool flag = false;

    for (auto _ : state) {
        state.PauseTiming();
        auto val = dis(gen);
        state.ResumeTiming();
        benchmark::DoNotOptimize(flag = s.count(val));
    }

    //    std::cout << flag << "\n";
}

 BENCHMARK(BM_search_map);
 BENCHMARK(BM_search_vector);

 BENCHMARK_MAIN()//
 Created by user on 25.12.2019.

