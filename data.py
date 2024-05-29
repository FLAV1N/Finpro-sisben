import requests
import csv
import time
from datetime import datetime

def get_data(url):
    response = requests.get(url)
    return response.text.replace('.', ',')

def main():
    start_time = datetime.now()
    filename = start_time.strftime("%d-%m-%Y %H-%M-%S.csv")
    with open(filename, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(['time', 'temperature', 'humidity'])  # write the header
        while True:
            temperature = get_data('http://192.168.100.186/temperature')
            humidity = get_data('http://192.168.100.186/humidity')
            data = [
                datetime.now().isoformat(),
                temperature,
                humidity
            ]
            writer.writerow(data)
            f.flush()  # ensure the data is written to the file immediately
            time.sleep(2)
            print("Saved!")

if __name__ == "__main__":
    main()
