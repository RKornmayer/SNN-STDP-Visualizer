#include <gtest/gtest.h>
#include "Neuron.h"
#include "SmallDatatypes.h"

// 1. Test: Verifies that a newly instantiated Neuron starts with correct defaults
TEST(NeuronTest, InitialStateIsCorrect) {
    // Arrange: Create a neuron at position 0,0
    position pos = { 0, 0 };
    Neuron n(pos);

    // Assert: Potential should be 0.0 and activity state should be 0 (resting)
    EXPECT_DOUBLE_EQ(n.getPotential(), 0.0);
    EXPECT_EQ(n.getActivity(), 0);
}

// 2. Test: Verifies that a signal below the threshold does NOT trigger a spike
TEST(NeuronTest, SubThresholdSignalDoesNotFire) {
    // Arrange
    position pos = { 0, 0 };
    Neuron n(pos);

    // Act: Inject a signal of 15.0, which is below the default threshold of 20.0
    n.addEPSP(15.0);
    n.updateActivity();

    // Assert: The neuron should remain in the resting state (0)
    EXPECT_EQ(n.getActivity(), 0);
}

// 3. Test: Verifies that reaching the threshold triggers the firing state
TEST(NeuronTest, FiresWhenThresholdReached) {
    // Arrange
    position pos = { 0, 0 };
    Neuron n(pos);

    // Act: Inject a massive signal that exceeds the default threshold (20.0)
    n.addEPSP(25.0);
    n.updateActivity();

    // Assert: Neuron activity should now be 1 (firing)
    EXPECT_EQ(n.getActivity(), 1);
}