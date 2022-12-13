import io
import random
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np


DATA_PATH = "./post_processing/data/"

def pop_whitespace(content: list[str]) -> str:
    """Removes any whitespace lines from a list of strings. Modifies in place and returns."""
    if len(content) == 0:
        return content

    while content[0].isspace():
        content.pop(0)

        if len(content) == 0:
            return content
    
    return content


class LoadCellOutput:

    def __init__(self, data: pd.DataFrame, assumed_time_increment=500) -> None:
        """If the data includes no time increment, assumed_time_increment will be used"""
        self.df = data
        self.df.dropna(inplace=True)

        # Add time if it is not already included
        if "t" not in self.df.columns:
            total = len(self.df)
            time_data = np.linspace(0, total * assumed_time_increment, total)
            self.df["t"] = time_data
        
    @staticmethod
    def from_string(content: str):
        lines = content.splitlines()

        # Remove calibration header and load calibration values
        del lines[0]
        calibration1 = float(lines.pop(0))
        calibration2 = float(lines.pop(0))
        calibration3 = float(lines.pop(0))

        lines = pop_whitespace(lines)

        df = pd.read_csv(io.StringIO("\n".join(lines)))
        df["cal1"] = [calibration1] * len(df)
        df["cal2"] = [calibration2] * len(df)
        df["cal3"] = [calibration3] * len(df)
        return LoadCellOutput(df)

    def to_csv(self, path: str):
        self.df.to_csv(path)


    def plot_mass(self):
        plt.plot(self.df["t"], self.df["sum"])
        plt.title("Mass Over Time")
        plt.ylabel("Mass (g)")
        plt.xlabel("Time (ms)")



DELIMETER = "Cals:"

def split_file_by_runs(path: str) -> list[LoadCellOutput]:
    """Reads the .txt located and path and finds all of the separate times the device was started and stopped. It splits each one into a separate file located in the data folder."""
    outputs = []

    with open(path, "r") as f:
        contents = f.read()

        contents = contents.split(DELIMETER)

        for content in contents:
            if content == "":
                continue

            output = LoadCellOutput.from_string(DELIMETER + content)
            outputs.append(output)

    return outputs


def save_all_runs(path: str) -> None:
    runs = split_file_by_runs(DATA_PATH + "durability_test.txt")

    for run in runs:
        run.to_csv(f"{DATA_PATH}{random.random()}.csv")


# Code to detect when a tank is loaded
# Code to detect when a tank is being drained
# Code to get total output over an interval

if __name__ == "__main__":
    output = LoadCellOutput(pd.read_csv(f"{DATA_PATH}durability_test.csv"))


    
    font = {'family' : 'normal',
        # 'weight' : 'bold',
        'size'   : 18}

    plt.rc('font', **font)

    output.plot_mass()


    plt.show()

    pass