import pandas as pd
import matplotlib.pyplot as plt
import sys

filename = sys.argv[1:][0]

if len(filename) == 0:
    print("No file provided")
    sys.exit()
else:
    data = pd.read_csv(f"{filename}", header=0)

    # Check if data has 2 columns
    if len(data.columns) != 2:
        print("Data must have 2 columns")
        sys.exit()

    # Check if data has x and y columns
    if "x" not in data.columns or "y" not in data.columns:
        print("Data must have x and y columns")
        sys.exit()

    plt.figure(figsize=(10, 5))
    plt.scatter(data["x"], data["y"])
    plt.xlabel("x")
    plt.ylabel("y")
    plt.title(filename)
    plt.show()
