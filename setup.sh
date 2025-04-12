#!/bin/bash

# Define the names of the folders to be created
folders=("fluxes" "cross_sections" "diffrate")


# Create the folders
for folder in "${folders[@]}"; do
  mkdir -p "$folder"
  echo "Created folder: $folder"
done

# Print a message
echo "Folders created successfully."

# Run the ROOT .C macro
echo "Executing the ROOT macro..."

# Replace 'yourMacro.C' with the actual name of your macro file
root -l -q make_Bollig_fluxes.C

# Check if the macro ran successfully
if [ $? -eq 0 ]; then
  echo "ROOT macro executed successfully!"
else
  echo "Failed to execute ROOT macro."
fi

echo "Script completed."
