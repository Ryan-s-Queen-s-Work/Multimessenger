import numpy as np

# Parameters
num_events = 1000  # Number of events
time_min, time_max = 0, 10e9  # Time range in ns
energy_mean, energy_sigma = 25, 1  # Energy mean (keV) and sigma (keV)

# Generate data
times = np.random.uniform(time_min, time_max, num_events)
energies = np.random.normal(energy_mean, energy_sigma, num_events)

# Save to file
with open("recoil_data.csv", "w") as f:
    for t, e in zip(times, energies):
        f.write(f"{format(t, '.1e')} {round(e, 1)}\n") # Shorten numbers (int for time, 1 decimal for energy)

print("File 'recoil_data.csv' generated successfully!")
