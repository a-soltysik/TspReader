#include <gtest/gtest.h>

#include "Reader.h"

TEST(ReaderTest, getTspContentTest)
{
    constexpr static auto instance = "NAME:  br17\n"
                                     "TYPE: ATSP\n"
                                     "COMMENT: 17 city problem (Repetto)\n"
                                     "DIMENSION:  17\n"
                                     "EDGE_WEIGHT_TYPE: EXPLICIT\n"
                                     "EDGE_WEIGHT_FORMAT: FULL_MATRIX\n"
                                     "UNKNOWN_TAG1: aaa\n"
                                     "UNKNOWN_TAG2\n"
                                     "EDGE_WEIGHT_SECTION:\n"
                                     " 9999    3    5   48   48    8    8    5    5    3    3    0    3    5    8    8\n"
                                     "    5\n"
                                     "    3 9999    3   48   48    8    8    5    5    0    0    3    0    3    8    8\n"
                                     "    5\n"
                                     "    5    3 9999   72   72   48   48   24   24    3    3    5    3    0   48   48\n"
                                     "   24\n"
                                     "   48   48   74 9999    0    6    6   12   12   48   48   48   48   74    6    6\n"
                                     "   12\n"
                                     "   48   48   74    0 9999    6    6   12   12   48   48   48   48   74    6    6\n"
                                     "   12\n"
                                     "    8    8   50    6    6 9999    0    8    8    8    8    8    8   50    0    0\n"
                                     "    8\n"
                                     "    8    8   50    6    6    0 9999    8    8    8    8    8    8   50    0    0\n"
                                     "    8\n"
                                     "    5    5   26   12   12    8    8 9999    0    5    5    5    5   26    8    8\n"
                                     "    0\n"
                                     "    5    5   26   12   12    8    8    0 9999    5    5    5    5   26    8    8\n"
                                     "    0\n"
                                     "    3    0    3   48   48    8    8    5    5 9999    0    3    0    3    8    8\n"
                                     "    5\n"
                                     "    3    0    3   48   48    8    8    5    5    0 9999    3    0    3    8    8\n"
                                     "    5\n"
                                     "    0    3    5   48   48    8    8    5    5    3    3 9999    3    5    8    8\n"
                                     "    5\n"
                                     "    3    0    3   48   48    8    8    5    5    0    0    3 9999    3    8    8\n"
                                     "    5\n"
                                     "    5    3    0   72   72   48   48   24   24    3    3    5    3 9999   48   48\n"
                                     "   24\n"
                                     "    8    8   50    6    6    0    0    8    8    8    8    8    8   50 9999    0\n"
                                     "    8\n"
                                     "    8    8   50    6    6    0    0    8    8    8    8    8    8   50    0 9999\n"
                                     "    8\n"
                                     "    5    5   26   12   12    8    8    0    0    5    5    5    5   26    8    8\n"
                                     " 9999\n"
                                     "EOF";

    const auto content = tsplib::getTspContent(instance);

    ASSERT_NO_THROW(content.value());

    const auto graph = content->graph;

    EXPECT_EQ(graph.getOrder(), 17);
    EXPECT_EQ(graph.getSize(), 17 * 17 - graph.getOrder());
    EXPECT_EQ(graph.getWeight({0, 3}).value(), 48);
    EXPECT_EQ(graph.getWeightUnchecked({2, 3}), 72);
    EXPECT_EQ(graph.getWeight({13, 12}).value(), 3);
}