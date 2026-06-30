#include <gtest/gtest.h>
#include "Synapse.h"
#include "Neuron.h"
#include "SmallDatatypes.h"

// Test 1: Verifies that the Synapse constructor links everything correctly
TEST(SynapseTest, InitializationIsCorrect) {
    // Arrange: Create two neurons to link
    position pos1 = { 0, 0 };
    position pos2 = { 1, 1 };
    Neuron source(pos1);
    Neuron target(pos2);
    double initialWeight = 1.5;

    // Act: Create the synapse
    Synapse synapse(&source, &target, initialWeight);

    // Assert: Check connections and weight
    EXPECT_DOUBLE_EQ(synapse.getWeight(), initialWeight);
    EXPECT_EQ(synapse.getSource(), &source);
    EXPECT_EQ(synapse.getTarget(), &target);
}

// Test 2: Verifies that an excitatory synapse (positive weight) increases target potential
TEST(SynapseTest, TransmitsExcitatorySignal) {
    // Arrange
    position pos1 = { 0, 0 };
    position pos2 = { 1, 1 };
    Neuron source(pos1);
    Neuron target(pos2);

    // Create an excitatory synapse
    Synapse synapse(&source, &target, 2.0);

    // Set source neuron to firing state (1), assuming transmit() checks this
    source.setActivity(1);

    // Act
    synapse.transmit();

    // Assert: Target potential should increase from 0.0 to 2.0
    EXPECT_DOUBLE_EQ(target.getPotential(), 2.0);
}

// Test 3: Verifies that an inhibitory synapse (negative weight) decreases target potential
TEST(SynapseTest, TransmitsInhibitorySignal) {
    // Arrange
    position pos1 = { 0, 0 };
    position pos2 = { 1, 1 };
    Neuron source(pos1);
    Neuron target(pos2);

    // Give the target an initial potential of 5.0 so we can reduce it cleanly
    target.addEPSP(5.0);

    // Create an inhibitory synapse with a negative weight
    Synapse synapse(&source, &target, -3.0);
    source.setActivity(1);

    // Act
    synapse.transmit();

    // Assert: Target potential should be reduced (5.0 - 3.0 = 2.0)
    EXPECT_DOUBLE_EQ(target.getPotential(), 2.0);
}