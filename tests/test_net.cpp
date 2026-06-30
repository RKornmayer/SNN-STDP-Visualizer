#include <gtest/gtest.h>
#include "Net.h"
#include "Neuron.h"
#include <vector>

// Test 1: Verifies that the network builds the correct number of layers and neurons
TEST(NetTest, InitializesTopologyCorrectly) {
    // 1. Arrange: Define a network with 3 layers (2 input, 3 hidden, 1 output)
    std::vector<size_t> topology = { 2, 3, 1 };

    // 2. Act: Instantiate the network
    Net network(topology);

    // 3. Assert: Verify the outer structure (number of layers)
    const auto& layers = network.getLayers();
    ASSERT_EQ(layers.size(), 3);

    // Verify the inner structure (number of neurons per layer)
    EXPECT_EQ(layers[0].size(), 2);
    EXPECT_EQ(layers[1].size(), 3);
    EXPECT_EQ(layers[2].size(), 1);
}

// Test 2: Verifies that a signal applied to the input layer propagates forward
TEST(NetTest, PropagatesSignalForward) {
    // 1. Arrange: Create a minimal 1-to-1 network
    std::vector<size_t> topology = { 1, 1 };
    Net network(topology);

    // 2. Act: Inject a stimulus into the first layer
    // We assume '1' sets the activity of the input neuron to firing state
    std::vector<short> input = { 1 };
    network.setInput(input);

    // Advance the simulation by one time step
    network.update();

    // 3. Assert: The target neuron (Layer 1, Index 0) should have received the signal.
    // Since we don't know the exact random starting weight, we just expect the potential 
    // to be strictly greater than 0.0 (assuming feedforward weights are excitatory).
    const auto& layers = network.getLayers();
    EXPECT_GT(layers[1][0].getPotential(), 0.0);
}