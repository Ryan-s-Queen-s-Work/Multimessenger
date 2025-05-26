import pandas as pd
import argparse

# Set up argument parser
parser = argparse.ArgumentParser(description="Process a CSV file and add row count to the filename.")
parser.add_argument("input", type=str, help="Path to the input CSV file")

# Parse arguments
args = parser.parse_args()
input_csv = args.input

# Read CSV file (assuming space-separated format, without headers)
df = pd.read_csv(input_csv, delim_whitespace=True, header=None)

# Sort by the first column (assuming time is in column index 0)
df = df.sort_values(by=0)

# Get total number of rows
num_rows = len(df)

# Construct dynamic filename
output_filename = f"{input_csv.split('.')[0]}_{num_rows}_rows.csv"

# Save to new CSV file without headers
df.to_csv(output_filename, index=False, sep=" ", header=False)

print(f"Processed file saved as '{output_filename}'")
