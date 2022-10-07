#include <gtest/gtest.h>

#include "parsers/SubParsers.h"

TEST(TspReaderTest, NameParserTest)
{
    EXPECT_EQ(tsp::name("NAME  :  CoolName\nCOMMENT")->first.name, "CoolName");
}

TEST(TspReaderTest, CommentParserTest)
{
    EXPECT_EQ(tsp::comment("COMMENT  :\tCoolComment\nNAME")->first.comment, "CoolComment");
}

TEST(TspReaderTest, TypeParserTest)
{
    EXPECT_EQ(tsp::type("TYPE \v:\tTSP")->first, tsp::AlgorithmType::TSP);
    EXPECT_EQ(tsp::type("TYPE \v:\tATSP")->first, tsp::AlgorithmType::ATSP);
    EXPECT_EQ(tsp::type("TYPE \v:\tSOP")->first, tsp::AlgorithmType::SOP);
    EXPECT_EQ(tsp::type("TYPE \v:\tHCP")->first, tsp::AlgorithmType::HCP);
    EXPECT_EQ(tsp::type("TYPE \v:\tCRVP")->first, tsp::AlgorithmType::CRVP);
    EXPECT_EQ(tsp::type("TYPE \v:\tTOUR")->first, tsp::AlgorithmType::TOUR);
}

TEST(TspReaderTest, EdgeWeightTypeParser)
{
    EXPECT_EQ(tsp::edgeWeightType("EDGE_WEIGHT_TYPE : EXPLICIT")->first, tsp::EdgeWeightType::EXPLICIT);
    EXPECT_EQ(tsp::edgeWeightType("EDGE_WEIGHT_TYPE : EUC_2D")->first, tsp::EdgeWeightType::EUC);
    EXPECT_EQ(tsp::edgeWeightType("EDGE_WEIGHT_TYPE : EUC_3D")->first, tsp::EdgeWeightType::EUC);
    EXPECT_EQ(tsp::edgeWeightType("EDGE_WEIGHT_TYPE : MAX_2D")->first, tsp::EdgeWeightType::MAX);
    EXPECT_EQ(tsp::edgeWeightType("EDGE_WEIGHT_TYPE : MAX_3D")->first, tsp::EdgeWeightType::MAX);
    EXPECT_EQ(tsp::edgeWeightType("EDGE_WEIGHT_TYPE : MAN_2D")->first, tsp::EdgeWeightType::MAN);
    EXPECT_EQ(tsp::edgeWeightType("EDGE_WEIGHT_TYPE : MAN_3D")->first, tsp::EdgeWeightType::MAN);
    EXPECT_EQ(tsp::edgeWeightType("EDGE_WEIGHT_TYPE : CEIL_2D")->first, tsp::EdgeWeightType::CEIL);
    EXPECT_EQ(tsp::edgeWeightType("EDGE_WEIGHT_TYPE : GEO")->first, tsp::EdgeWeightType::GEO);
}

TEST(TspReaderTest, Node2dParser)
{
    auto node = tsp::node2d("1 2 3\n")->first;
    EXPECT_EQ(node.id, 1);
    EXPECT_FLOAT_EQ(node.x, 2);
    EXPECT_FLOAT_EQ(node.y, 3);

    node = tsp::node2d("3245 345.3 -0.234\n")->first;
    EXPECT_EQ(node.id, 3245);
    EXPECT_FLOAT_EQ(node.x, 345.3f);
    EXPECT_FLOAT_EQ(node.y, -0.234f);

    ASSERT_THROW(tsp::node2d("1.0 345.3 -0.234\n").value(), std::bad_optional_access);
    ASSERT_THROW(tsp::node2d("1 345.3 \n -0.234\n").value(), std::bad_optional_access);
    ASSERT_THROW(tsp::node2d("1 36266.6667 62550.0000 3.0\n").value(), std::bad_optional_access);
}

TEST(TspReaderTest, Node3dParser)
{
    auto node = tsp::node3d("1 2 3 4\n")->first;
    EXPECT_EQ(node.id, 1);
    EXPECT_FLOAT_EQ(node.x, 2);
    EXPECT_FLOAT_EQ(node.y, 3);
    EXPECT_FLOAT_EQ(node.z, 4);

    node = tsp::node3d("3245 345.3 -0.234 21\n")->first;
    EXPECT_EQ(node.id, 3245);
    EXPECT_FLOAT_EQ(node.x, 345.3f);
    EXPECT_FLOAT_EQ(node.y, -0.234f);
    EXPECT_FLOAT_EQ(node.z, 21);

    ASSERT_THROW(tsp::node3d("1.0 345.3 -0.234 23\n").value(), std::bad_optional_access);
    ASSERT_THROW(tsp::node3d("1 345.3 -0.234\n").value(), std::bad_optional_access);
    ASSERT_THROW(tsp::node3d("1 345.3 -0.234\n 3\n").value(), std::bad_optional_access);
}

TEST(TspReaderTest, NodeCoordSection)
{
    using namespace std::string_view_literals;
    static constexpr auto nodes2d = "NODE_COORD_SECTION:\n"
                                                   "1 36266.6667 62550.0000\n"
                                                   "2 34600.0000 58633.3333\n"
                                                   "3 51650.0000 72300.0000\n"
                                                   "4 37800.0000 67683.3333\n"sv;

    auto nodes2dParsed = tsp::nodeCoordSection(nodes2d);
    EXPECT_NO_THROW(nodes2dParsed.value());
    EXPECT_NO_THROW(std::get<tsp::Nodes2d>(nodes2dParsed->first.nodesCoord));
    EXPECT_EQ(std::get<tsp::Nodes2d>(nodes2dParsed->first.nodesCoord).nodes2d.size(), 4);
    EXPECT_FLOAT_EQ(std::get<tsp::Nodes2d>(nodes2dParsed->first.nodesCoord).nodes2d[2].y, 72300);

    static constexpr auto nodes3d = "NODE_COORD_SECTION:\n"
                                                   "1 36266.6667 62550.0000 3.0\n"
                                                   "2 34600.0000 58633.3333 2.3\n"
                                                   "3 51650.0000 72300.0000 9.2\n"
                                                   "4 37800.0000 67683.3333 5\n"sv;
    auto nodes3dParsed = tsp::nodeCoordSection(nodes3d);

    EXPECT_NO_THROW(nodes3dParsed.value());
    EXPECT_NO_THROW(std::get<tsp::Nodes3d>(nodes3dParsed->first.nodesCoord));
    EXPECT_EQ(std::get<tsp::Nodes3d>(nodes3dParsed->first.nodesCoord).nodes3d.size(), 4);
    EXPECT_FLOAT_EQ(std::get<tsp::Nodes3d>(nodes3dParsed->first.nodesCoord).nodes3d[2].z, 9.2f);

    static constexpr auto nodesError = "NODE_COORD_SECTION:\n"
                                                      "1 36266.6667\n"
                                                      "2 34600.0000\n"
                                                      "3 51650.0000\n"
                                                      "4 37800.0000\n"sv;
    nodes2dParsed = tsp::nodeCoordSection(nodesError);

    EXPECT_THROW(nodes2dParsed.value(), std::bad_optional_access);

    static constexpr auto nodesMix = "NODE_COORD_SECTION:\n"
                                                    "1 36266.6667 62550.0000\n"
                                                    "2 34600.0000 58633.3333 2.3\n"
                                                    "3 51650.0000 72300.0000\n"
                                                    "4 37800.0000 67683.3333 5\n"sv;

    nodes2dParsed = tsp::nodeCoordSection(nodesMix);

    EXPECT_NO_THROW(nodes2dParsed.value());
    EXPECT_NO_THROW(std::get<tsp::Nodes2d>(nodes2dParsed->first.nodesCoord));
    EXPECT_EQ(std::get<tsp::Nodes2d>(nodes2dParsed->first.nodesCoord).nodes2d.size(), 1);
    EXPECT_FLOAT_EQ(std::get<tsp::Nodes2d>(nodes2dParsed->first.nodesCoord).nodes2d[0].y, 62550.0000f);

    EXPECT_THROW(std::get<tsp::Nodes3d>(nodes2dParsed->first.nodesCoord), std::bad_variant_access);
}