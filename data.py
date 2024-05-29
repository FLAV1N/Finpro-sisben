import requests  # Mengimpor modul requests untuk melakukan permintaan HTTP
import csv  # Mengimpor modul csv untuk menulis file CSV
import time  # Mengimpor modul time untuk fungsi sleep
from datetime import datetime  # Mengimpor kelas datetime dari modul datetime untuk mendapatkan waktu saat ini

def get_data(url):  # Fungsi untuk mendapatkan data dari URL
    response = requests.get(url)  # Melakukan permintaan GET ke URL
    return response.text.replace('.', ',')  # Mengembalikan teks respons dengan mengganti titik dengan koma

def main():  # Fungsi utama
    start_time = datetime.now()  # Mendapatkan waktu saat script dimulai
    filename = start_time.strftime("%d-%m-%Y %H-%M-%S.csv")  # Membuat nama file dengan format tanggal dan waktu
    with open(filename, 'w', newline='') as f:  # Membuka file untuk ditulis
        writer = csv.writer(f)  # Membuat objek penulis CSV
        writer.writerow(['time', 'temperature', 'humidity'])  # Menulis header
        while True:  # Loop tak terbatas
            temperature = get_data('http://192.168.100.186/temperature')  # Mendapatkan data suhu
            humidity = get_data('http://192.168.100.186/humidity')  # Mendapatkan data kelembaban
            data = [  # Membuat list data
                datetime.now().isoformat(),  # Waktu saat ini
                temperature,  # Suhu
                humidity  # Kelembaban
            ]
            writer.writerow(data)  # Menulis data ke file CSV
            f.flush()  # Memastikan data ditulis ke file segera
            time.sleep(2)  # Menunggu 2 detik
            print("Saved!")  # Mencetak pesan "Saved!"

if __name__ == "__main__":
    main()  # Menjalankan fungsi utama jika script dijalankan sebagai program utama
