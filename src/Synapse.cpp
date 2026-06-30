#include "Synapse.h"
#include "Neuron.h"
#include <algorithm>

Synapse::Synapse(Neuron* src, Neuron* tgt, double initialWeight, bool plastic)
	: source(src), target(tgt), weight(initialWeight), isPlastic(plastic)
{
}

void Synapse::transmit()
{
	if (target)
	{
		target->addEPSP(weight);
	}
}

void Synapse::applyPlasticity()
{
	// Exclude static (e.g., lateral inhibitory) synapses from plasticity rules
	if (!isPlastic)
	{
		return;
	}

	if (source && target)
	{
		short srcAct = source->getActivity();
		short tgtAct = target->getActivity();

		// 1. Causality / Long-Term Potentiation (LTP)
		if (tgtAct == 1 && srcAct == 2)
		{
			weight *= 1.2;
		}
		// 2. Synchrony / Mild Long-Term Depression (LTD)
		else if (tgtAct == 1 && srcAct == 1)
		{
			weight *= 0.95;
		}
		// 3. Anti-Causality / Strong Long-Term Depression (LTD)
		else if (tgtAct == 2 && srcAct == 1)
		{
			weight *= 0.8;
		}

		// Hard-cap synaptic weights to simulate biological AMPA receptor density limits
		weight = std::max(0.025, std::min(0.6, weight));
	}
}