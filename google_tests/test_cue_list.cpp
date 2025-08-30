#include <gtest/gtest.h>
#include "../src/cue_list.h"

class CueList;

class CueListUpdateTest : public ::testing::Test {
protected:
    void SetUp() override {
        cue_list = new CueList(5);  // Creates cues 1, 2, 3

        // Set up some initial moves
        cue_list->update_channel(1, 100, CueList::UpdateMode::UPDATE_TRACK, 2);
        cue_list->update_channel(3, 50, CueList::UpdateMode::UPDATE_TRACK, 3);
        cue_list->update_channel(5, 50, CueList::UpdateMode::UPDATE_TRACK, 1);
        cue_list->update_channel(2, 50, CueList::UpdateMode::UPDATE_TRACK, 1);
        cue_list->update_channel(2, 10, CueList::UpdateMode::UPDATE_TRACK, 3);


    }

    void TearDown() override {
        delete cue_list;
    }

    CueList* cue_list{};
};

TEST_F(CueListUpdateTest, UpdateTrace) {
    // Test UPDATE_TRACE
    cue_list->update_channel(1, 60, CueList::UpdateMode::UPDATE_TRACE, 5);

    EXPECT_EQ(-1, cue_list->get_channel_value_at_cue(1, 1));
    EXPECT_EQ(60, cue_list->get_channel_value_at_cue(1, 2));
    EXPECT_EQ(60, cue_list->get_channel_value_at_cue(1, 3));
    EXPECT_EQ(60, cue_list->get_channel_value_at_cue(1, 4));
    EXPECT_EQ(60, cue_list->get_channel_value_at_cue(1, 5));

}
TEST_F(CueListUpdateTest, UpdateTraceTrace) {
    // Test UPDATE_TRACE
    cue_list->update_channel(3, 60, CueList::UpdateMode::UPDATE_TRACE_TRACE, 5);

    EXPECT_EQ(60, cue_list->get_channel_value_at_cue(3, 1));
    EXPECT_EQ(60, cue_list->get_channel_value_at_cue(3, 2));
    EXPECT_EQ(60, cue_list->get_channel_value_at_cue(3, 3));
    EXPECT_EQ(60, cue_list->get_channel_value_at_cue(3, 4));
    EXPECT_EQ(60, cue_list->get_channel_value_at_cue(3, 5));

}

TEST_F(CueListUpdateTest, UpdateTrackNoExistingMoves) {
    // Test UPDATE_TRACK - no existing moves
    cue_list->update_channel(4, 80, CueList::UpdateMode::UPDATE_TRACK, 1);

    EXPECT_EQ(80, cue_list->get_channel_value_at_cue(4, 1));
    EXPECT_EQ(80, cue_list->get_channel_value_at_cue(4, 2));
    EXPECT_EQ(80, cue_list->get_channel_value_at_cue(4, 3));
    EXPECT_EQ(80, cue_list->get_channel_value_at_cue(4, 4));
    EXPECT_EQ(80, cue_list->get_channel_value_at_cue(4, 5));
}

TEST_F(CueListUpdateTest, UpdateTrackHitMove) {
    // Test UPDATE_TRACK - hits a later move
    cue_list->update_channel(3, 80, CueList::UpdateMode::UPDATE_TRACK, 1);

    EXPECT_EQ(80, cue_list->get_channel_value_at_cue(3, 1));
    EXPECT_EQ(80, cue_list->get_channel_value_at_cue(3, 2));
    EXPECT_EQ(50, cue_list->get_channel_value_at_cue(3, 3));
    EXPECT_EQ(50, cue_list->get_channel_value_at_cue(3, 4));
    EXPECT_EQ(50, cue_list->get_channel_value_at_cue(3, 5));

}



TEST_F(CueListUpdateTest, UpdateCueOnlyNullValues) {
    // Test UPDATE_CUE_ONLY

    cue_list->update_channel(4, 90, CueList::UpdateMode::UPDATE_CUE_ONLY, 3);

    EXPECT_EQ(-1, cue_list->get_channel_value_at_cue(4, 1));
    EXPECT_EQ(-1, cue_list->get_channel_value_at_cue(4, 2));
    EXPECT_EQ(90, cue_list->get_channel_value_at_cue(4, 3));
    EXPECT_EQ(-1, cue_list->get_channel_value_at_cue(4, 4));
    EXPECT_EQ(-1, cue_list->get_channel_value_at_cue(4, 5));

}

TEST_F(CueListUpdateTest, UpdateCueOnlyValues) {
    // Test UPDATE_CUE_ONLY

    cue_list->update_channel(1, 90, CueList::UpdateMode::UPDATE_CUE_ONLY, 3);

    EXPECT_EQ(-1, cue_list->get_channel_value_at_cue(1, 1));
    EXPECT_EQ(100, cue_list->get_channel_value_at_cue(1, 2));
    EXPECT_EQ(90, cue_list->get_channel_value_at_cue(1, 3));
    EXPECT_EQ(100, cue_list->get_channel_value_at_cue(1, 4));
    EXPECT_EQ(100, cue_list->get_channel_value_at_cue(1, 5));
}


TEST_F(CueListUpdateTest, ColorUp) {
    EXPECT_STREQ("blue", cue_list->get_channel_color_at_cue(1, 2).c_str());
}
TEST_F(CueListUpdateTest, ColorDown) {
    EXPECT_STREQ("green", cue_list->get_channel_color_at_cue(2, 3).c_str());
}
TEST_F(CueListUpdateTest, ColorTrack) {
    EXPECT_STREQ("magenta", cue_list->get_channel_color_at_cue(1, 3).c_str());
}


TEST_F(CueListUpdateTest, InvalidChannel) {
    EXPECT_THROW(
        cue_list->update_channel(999, 50, CueList::UpdateMode::UPDATE_TRACK, 1),
        std::invalid_argument
    );
}