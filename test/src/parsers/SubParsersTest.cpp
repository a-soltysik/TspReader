#include <gtest/gtest.h>

#include "parsers/SubParsers.h"

TEST(TspReaderTest, NameParserTest)
{
    EXPECT_EQ(std::get<tsplib::Name>(tsplib::name(" CoolName\nCOMMENT")->first).name, "CoolName");
}

TEST(TspReaderTest, CommentParserTest)
{
    EXPECT_EQ(std::get<tsplib::Comment>(tsplib::comment("\tCoolComment\nNAME")->first).comment, "CoolComment");
}

TEST(TspReaderTest, TypeParserTest)
{
    EXPECT_EQ(std::get<tsplib::AlgorithmType>(tsplib::type("\tTSP")->first), tsplib::AlgorithmType::TSP);
    EXPECT_EQ(std::get<tsplib::AlgorithmType>(tsplib::type("\tATSP")->first), tsplib::AlgorithmType::ATSP);
    EXPECT_EQ(std::get<tsplib::AlgorithmType>(tsplib::type("\tSOP")->first), tsplib::AlgorithmType::SOP);
    EXPECT_EQ(std::get<tsplib::AlgorithmType>(tsplib::type("\tHCP")->first), tsplib::AlgorithmType::HCP);
    EXPECT_EQ(std::get<tsplib::AlgorithmType>(tsplib::type("\tCRVP")->first), tsplib::AlgorithmType::CRVP);
    EXPECT_EQ(std::get<tsplib::AlgorithmType>(tsplib::type("\tTOUR")->first), tsplib::AlgorithmType::TOUR);
}

TEST(TspReaderTest, EdgeDataFormatParser)
{
    EXPECT_EQ(std::get<tsplib::EdgeDataFormat>(tsplib::edgeDataFormat(" EDGE_LIST\n")->first),
              tsplib::EdgeDataFormat::EDGE_LIST);
    EXPECT_EQ(std::get<tsplib::EdgeDataFormat>(tsplib::edgeDataFormat(" ADJ_LIST\n")->first),
              tsplib::EdgeDataFormat::ADJ_LIST);
}

TEST(TspReaderTest, DimensionParserTest)
{
    EXPECT_EQ(std::get<tsplib::Dimension>(tsplib::dimension(" 45")->first).dimension, 45);
    EXPECT_EQ(std::get<tsplib::Dimension>(tsplib::dimension("1001")->first).dimension, 1001);
    EXPECT_EQ(std::get<tsplib::Dimension>(tsplib::dimension(" 1001.45")->first).dimension, 1001);
    EXPECT_THROW(tsplib::dimension("-1001").value(), std::bad_optional_access);
}

TEST(TspReaderTest, CapacityParserTest)
{
    EXPECT_EQ(std::get<tsplib::Capacity>(tsplib::capacity("45")->first).capacity, 45);
    EXPECT_EQ(std::get<tsplib::Capacity>(tsplib::capacity(" 1001")->first).capacity, 1001);
    EXPECT_EQ(std::get<tsplib::Capacity>(tsplib::capacity(" 1001.45")->first).capacity, 1001);
    EXPECT_THROW(tsplib::capacity(" -1001").value(), std::bad_optional_access);
}

TEST(TspReaderTest, EdgeWeightTypeParser)
{
    EXPECT_EQ(std::get<tsplib::EdgeWeightType>(tsplib::edgeWeightType(" EXPLICIT")->first),
              tsplib::EdgeWeightType::EXPLICIT);
    EXPECT_EQ(std::get<tsplib::EdgeWeightType>(tsplib::edgeWeightType(" EUC_2D")->first), tsplib::EdgeWeightType::EUC);
    EXPECT_EQ(std::get<tsplib::EdgeWeightType>(tsplib::edgeWeightType(" EUC_3D")->first), tsplib::EdgeWeightType::EUC);
    EXPECT_EQ(std::get<tsplib::EdgeWeightType>(tsplib::edgeWeightType(" MAX_2D")->first), tsplib::EdgeWeightType::MAX);
    EXPECT_EQ(std::get<tsplib::EdgeWeightType>(tsplib::edgeWeightType(" MAX_3D")->first), tsplib::EdgeWeightType::MAX);
    EXPECT_EQ(std::get<tsplib::EdgeWeightType>(tsplib::edgeWeightType(" MAN_2D")->first), tsplib::EdgeWeightType::MAN);
    EXPECT_EQ(std::get<tsplib::EdgeWeightType>(tsplib::edgeWeightType(" MAN_3D")->first), tsplib::EdgeWeightType::MAN);
    EXPECT_EQ(std::get<tsplib::EdgeWeightType>(tsplib::edgeWeightType(" CEIL_2D")->first),
              tsplib::EdgeWeightType::CEIL);
    EXPECT_EQ(std::get<tsplib::EdgeWeightType>(tsplib::edgeWeightType(" GEO")->first), tsplib::EdgeWeightType::GEO);
}

TEST(TspReaderTest, EdgeWeightFormatParser)
{
    EXPECT_EQ(std::get<tsplib::EdgeWeightFormat>(tsplib::edgeWeightFormat(" FUNCTION \n")->first),
              tsplib::EdgeWeightFormat::FUNCTION);
    EXPECT_EQ(std::get<tsplib::EdgeWeightFormat>(tsplib::edgeWeightFormat(" FULL_MATRIX \n")->first),
              tsplib::EdgeWeightFormat::FULL_MATRIX);
    EXPECT_EQ(std::get<tsplib::EdgeWeightFormat>(tsplib::edgeWeightFormat(" UPPER_ROW \n")->first),
              tsplib::EdgeWeightFormat::UPPER_ROW);
    EXPECT_EQ(std::get<tsplib::EdgeWeightFormat>(tsplib::edgeWeightFormat(" LOWER_ROW \n")->first),
              tsplib::EdgeWeightFormat::LOWER_ROW);
    EXPECT_EQ(std::get<tsplib::EdgeWeightFormat>(tsplib::edgeWeightFormat(" UPPER_DIAG_ROW \n")->first),
              tsplib::EdgeWeightFormat::UPPER_DIAG_ROW);
    EXPECT_EQ(std::get<tsplib::EdgeWeightFormat>(tsplib::edgeWeightFormat(" LOWER_DIAG_ROW \n")->first),
              tsplib::EdgeWeightFormat::LOWER_DIAG_ROW);
    EXPECT_EQ(std::get<tsplib::EdgeWeightFormat>(tsplib::edgeWeightFormat(" UPPER_COL \n")->first),
              tsplib::EdgeWeightFormat::UPPER_COL);
    EXPECT_EQ(std::get<tsplib::EdgeWeightFormat>(tsplib::edgeWeightFormat(" LOWER_COL \n")->first),
              tsplib::EdgeWeightFormat::LOWER_COL);
    EXPECT_EQ(std::get<tsplib::EdgeWeightFormat>(tsplib::edgeWeightFormat(" UPPER_DIAG_COL \n")->first),
              tsplib::EdgeWeightFormat::UPPER_DIAG_COL);
    EXPECT_EQ(std::get<tsplib::EdgeWeightFormat>(tsplib::edgeWeightFormat(" LOWER_DIAG_COL \n")->first),
              tsplib::EdgeWeightFormat::LOWER_DIAG_COL);
}

TEST(TspReaderTest, Node2dParser)
{
    auto node = tsplib::node2d("1 2 3\n")->first;
    EXPECT_EQ(node.id, 1);
    EXPECT_DOUBLE_EQ(node.x, 2);
    EXPECT_DOUBLE_EQ(node.y, 3);

    node = tsplib::node2d("3245 345.3 -0.234\n")->first;
    EXPECT_EQ(node.id, 3245);
    EXPECT_DOUBLE_EQ(node.x, 345.3);
    EXPECT_DOUBLE_EQ(node.y, -0.234);

    ASSERT_THROW(tsplib::node2d("1.0 345.3 -0.234\n").value(), std::bad_optional_access);
    ASSERT_THROW(tsplib::node2d("1 345.3 \n -0.234\n").value(), std::bad_optional_access);
    ASSERT_THROW(tsplib::node2d("1 36266.6667 62550.0000 3.0\n").value(), std::bad_optional_access);
}

TEST(TspReaderTest, Node3dParser)
{
    auto node = tsplib::node3d("1 2 3 4\n")->first;
    EXPECT_EQ(node.id, 1);
    EXPECT_DOUBLE_EQ(node.x, 2);
    EXPECT_DOUBLE_EQ(node.y, 3);
    EXPECT_DOUBLE_EQ(node.z, 4);

    node = tsplib::node3d("3245 345.3 -0.234 21\n")->first;
    EXPECT_EQ(node.id, 3245);
    EXPECT_DOUBLE_EQ(node.x, 345.3);
    EXPECT_DOUBLE_EQ(node.y, -0.234);
    EXPECT_DOUBLE_EQ(node.z, 21);

    ASSERT_THROW(tsplib::node3d("1.0 345.3 -0.234 23\n").value(), std::bad_optional_access);
    ASSERT_THROW(tsplib::node3d("1 345.3 -0.234\n").value(), std::bad_optional_access);
    ASSERT_THROW(tsplib::node3d("1 345.3 -0.234\n 3\n").value(), std::bad_optional_access);
}

TEST(TspReaderTest, NodeCoordSection)
{
    using namespace std::string_view_literals;
    static constexpr auto nodes2d = "\n"
                                    "1 36266.6667 62550.0000\n"
                                    "2 34600.0000 58633.3333\n"
                                    "3 51650.0000 72300.0000\n"
                                    "4 37800.0000 67683.3333\n"sv;

    auto nodes2dParsed = tsplib::nodeCoordSection(nodes2d);
    EXPECT_NO_THROW(nodes2dParsed.value());
    EXPECT_NO_THROW(std::get<tsplib::Nodes2d>(std::get<tsplib::NodeCoordSection>(nodes2dParsed->first).nodesCoord));
    EXPECT_EQ(
        std::get<tsplib::Nodes2d>(std::get<tsplib::NodeCoordSection>(nodes2dParsed->first).nodesCoord).nodes2d.size(),
        4);
    EXPECT_DOUBLE_EQ(
        std::get<tsplib::Nodes2d>(std::get<tsplib::NodeCoordSection>(nodes2dParsed->first).nodesCoord).nodes2d[2].y,
        72300);

    static constexpr auto nodes3d = "\n"
                                    "1 36266.6667 62550.0000 3.0\n"
                                    "2 34600.0000 58633.3333 2.3\n"
                                    "3 51650.0000 72300.0000 9.2\n"
                                    "4 37800.0000 67683.3333 5\n"sv;

    auto nodes3dParsed = tsplib::nodeCoordSection(nodes3d);

    EXPECT_NO_THROW(nodes3dParsed.value());
    EXPECT_NO_THROW(std::get<tsplib::Nodes3d>(std::get<tsplib::NodeCoordSection>(nodes3dParsed->first).nodesCoord));
    EXPECT_EQ(
        std::get<tsplib::Nodes3d>(std::get<tsplib::NodeCoordSection>(nodes3dParsed->first).nodesCoord).nodes3d.size(),
        4);
    EXPECT_DOUBLE_EQ(
        std::get<tsplib::Nodes3d>(std::get<tsplib::NodeCoordSection>(nodes3dParsed->first).nodesCoord).nodes3d[2].z,
        9.2);

    static constexpr auto nodesError = "\n"
                                       "1 36266.6667\n"
                                       "2 34600.0000\n"
                                       "3 51650.0000\n"
                                       "4 37800.0000\n"sv;

    nodes2dParsed = tsplib::nodeCoordSection(nodesError);

    EXPECT_THROW(nodes2dParsed.value(), std::bad_optional_access);

    static constexpr auto nodesMix = "\n"
                                     "1 36266.6667 62550.0000\n"
                                     "2 34600.0000 58633.3333 2.3\n"
                                     "3 51650.0000 72300.0000\n"
                                     "4 37800.0000 67683.3333 5\n"sv;

    nodes2dParsed = tsplib::nodeCoordSection(nodesMix);

    EXPECT_NO_THROW(nodes2dParsed.value());
    EXPECT_NO_THROW(std::get<tsplib::Nodes2d>(std::get<tsplib::NodeCoordSection>(nodes2dParsed->first).nodesCoord));
    EXPECT_EQ(
        std::get<tsplib::Nodes2d>(std::get<tsplib::NodeCoordSection>(nodes2dParsed->first).nodesCoord).nodes2d.size(),
        1);
    EXPECT_DOUBLE_EQ(
        std::get<tsplib::Nodes2d>(std::get<tsplib::NodeCoordSection>(nodes2dParsed->first).nodesCoord).nodes2d[0].y,
        62550.0000);

    EXPECT_THROW(std::get<tsplib::Nodes3d>(std::get<tsplib::NodeCoordSection>(nodes2dParsed->first).nodesCoord),
                 std::bad_variant_access);
}

TEST(TspReaderTest, edgeDataSectionParser)
{
    using namespace std::string_view_literals;

    static constexpr auto edgeList = "\n"
                                     "1 1\n"
                                     "2 2\n"
                                     "3 5\n"
                                     "-1\n"sv;

    static constexpr auto adjList = "\n"
                                    "1 2 3 4 5 -1\n"
                                    "2 1 3 4 -1\n"
                                    "3 2 4 5 -1\n"
                                    "-1\n"sv;

    const auto edgeDataResult = tsplib::edgeDataSection(edgeList);
    ASSERT_NO_THROW(edgeDataResult.value());
    ASSERT_NO_THROW(std::get<tsplib::EdgeDataSection>(edgeDataResult->first));

    const auto edgeData = std::get<tsplib::EdgeDataSection>(edgeDataResult->first).edgeData;

    ASSERT_EQ(edgeData.size(), 7);
    EXPECT_EQ(edgeData.front(), 1);
    EXPECT_EQ(edgeData.back(), -1);


    const auto adjDataResult = tsplib::edgeDataSection(adjList);
    ASSERT_NO_THROW(adjDataResult.value());
    ASSERT_NO_THROW(std::get<tsplib::EdgeDataSection>(adjDataResult->first));

    const auto adjData = std::get<tsplib::EdgeDataSection>(adjDataResult->first).edgeData;

    ASSERT_EQ(adjData.size(), 17);
    EXPECT_EQ(adjData.front(), 1);
    EXPECT_EQ(adjData.back(), -1);

}

TEST(TspReaderTest, tspDataParser)
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

    const auto config = tsplib::tspData(instance);
    ASSERT_NO_THROW(config.value());
    ASSERT_NO_THROW(config->first.specification.name.value());
    EXPECT_EQ(config->first.specification.name.value(), "br17");

    ASSERT_NO_THROW(config->first.specification.comment.value());
    EXPECT_EQ(config->first.specification.comment.value(), "17 city problem (Repetto)");

    ASSERT_NO_THROW(config->first.specification.name.value());
    EXPECT_EQ(config->first.specification.name.value(), "br17");

    ASSERT_NO_THROW(config->first.specification.dimension.value());
    EXPECT_EQ(config->first.specification.dimension.value(), 17);

    ASSERT_NO_THROW(config->first.specification.edgeWeightType.value());
    EXPECT_EQ(config->first.specification.edgeWeightType.value(), tsplib::EdgeWeightType::EXPLICIT);

    ASSERT_NO_THROW(config->first.specification.edgeWeightFormat.value());
    EXPECT_EQ(config->first.specification.edgeWeightFormat.value(), tsplib::EdgeWeightFormat::FULL_MATRIX);

    ASSERT_NO_THROW(config->first.data.edgeWeightSection.value());

    const auto edgeWeightSection = config->first.data.edgeWeightSection.value();
    EXPECT_EQ(edgeWeightSection.size(), 17 * 17);
    EXPECT_EQ(edgeWeightSection.front(), 9999);
    EXPECT_EQ(edgeWeightSection.back(), 9999);
}