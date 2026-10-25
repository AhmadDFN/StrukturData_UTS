#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

const int KAPASITAS_MENU = 10;
const int KAPASITAS_PESANAN = 10;

struct Menu {
    int kode;
    string nama;
    int harga;
};

struct Pesanan {
    string nama;
    int harga;
    int jumlah;
    long long subtotal;
};

void tampilkanMenu(const Menu daftarMenu[], int jumlahMenu) {
    cout << "\n============= DAFTAR MENU =============\n";
    if (jumlahMenu == 0) {
        cout << "Belum ada menu.\n";
        return;
    }

    cout << left << setw(8) << "Kode" << setw(24) << "Nama" << "Harga\n";
    cout << "---------------------------------------\n";
    for (int i = 0; i < jumlahMenu; i++) {
        cout << left << setw(8) << daftarMenu[i].kode
             << setw(24) << daftarMenu[i].nama
             << "Rp " << daftarMenu[i].harga << '\n';
    }
}

int cariMenu(const Menu daftarMenu[], int jumlahMenu, int kode) {
    // Cari kode menu satu per satu menggunakan perulangan.
    for (int i = 0; i < jumlahMenu; i++) {
        if (daftarMenu[i].kode == kode) {
            return i;
        }
    }
    return -1;
}

void tambahMenu(Menu daftarMenu[], int &jumlahMenu, int &kodeBerikutnya) {
    if (jumlahMenu >= KAPASITAS_MENU) {
        cout << "Kapasitas menu sudah penuh.\n";
        return;
    }

    cin.ignore();
    cout << "Masukkan nama menu: ";
    getline(cin, daftarMenu[jumlahMenu].nama);
    cout << "Masukkan harga menu: Rp. ";
    cin >> daftarMenu[jumlahMenu].harga;

    if (daftarMenu[jumlahMenu].harga <= 0) {
        cout << "Harga harus lebih dari 0. Menu tidak ditambahkan.\n";
        return;
    }

    daftarMenu[jumlahMenu].kode = kodeBerikutnya;
    cout << "Menu berhasil ditambahkan dengan kode " << kodeBerikutnya << ".\n";
    kodeBerikutnya++;
    jumlahMenu++;
}

void ubahMenu(Menu daftarMenu[], int jumlahMenu) {
    int kode;
    cout << "Masukkan kode menu yang akan diubah: ";
    cin >> kode;
    int posisi = cariMenu(daftarMenu, jumlahMenu, kode);

    if (posisi == -1) {
        cout << "Kode menu tidak ditemukan.\n";
    } else {
        cin.ignore();
        cout << "Nama baru: ";
        getline(cin, daftarMenu[posisi].nama);
        do {
            cout << "Harga baru (lebih dari 0): Rp. ";
            cin >> daftarMenu[posisi].harga;
            if (daftarMenu[posisi].harga <= 0) {
                cout << "Harga harus lebih dari 0.\n";
            }
        } while (daftarMenu[posisi].harga <= 0);
        cout << "Menu berhasil diubah.\n";
    }
}

void hapusMenu(Menu daftarMenu[], int &jumlahMenu) {
    int kode;
    cout << "Masukkan kode menu yang akan dihapus: ";
    cin >> kode;
    int posisi = cariMenu(daftarMenu, jumlahMenu, kode);

    if (posisi == -1) {
        cout << "Kode menu tidak ditemukan.\n";
    } else {
        // Geser data setelah posisi yang dihapus agar tetap berurutan.
        for (int i = posisi; i < jumlahMenu - 1; i++) {
            daftarMenu[i] = daftarMenu[i + 1];
        }
        jumlahMenu--;
        cout << "Menu berhasil dihapus.\n";
    }
}

void transaksi(const Menu daftarMenu[], int jumlahMenu) {
    if (jumlahMenu == 0) {
        cout << "Belum ada menu untuk ditransaksikan.\n";
        return;
    }

    Pesanan daftarPesanan[KAPASITAS_PESANAN];
    int jumlahPesanan = 0;
    char tambahLagi;

    do {
        tampilkanMenu(daftarMenu, jumlahMenu);
        int kode, jumlah;
        cout << "Masukkan kode menu yang dibeli: ";
        cin >> kode;
        int posisi = cariMenu(daftarMenu, jumlahMenu, kode);

        if (posisi == -1) {
            cout << "Kode menu tidak ditemukan.\n";
        } else if (jumlahPesanan >= KAPASITAS_PESANAN) {
            cout << "Kapasitas pesanan sudah penuh.\n";
        } else {
            cout << "Jumlah pembelian: ";
            cin >> jumlah;
            if (jumlah <= 0) {
                cout << "Jumlah harus lebih dari 0. Pesanan tidak ditambahkan.\n";
            } else {
                daftarPesanan[jumlahPesanan].nama = daftarMenu[posisi].nama;
                daftarPesanan[jumlahPesanan].harga = daftarMenu[posisi].harga;
                daftarPesanan[jumlahPesanan].jumlah = jumlah;
                daftarPesanan[jumlahPesanan].subtotal =
                    static_cast<long long>(daftarMenu[posisi].harga) * jumlah;
                jumlahPesanan++;
            }
        }

        cout << "Tambah pesanan lagi? (y/n): ";
        cin >> tambahLagi;
    } while (tambahLagi == 'y' || tambahLagi == 'Y');

    if (jumlahPesanan == 0) {
        cout << "Tidak ada pesanan. Transaksi dibatalkan.\n";
        return;
    }

    long long total = 0;
    cout << "\n================================\n";
    cout << "          STRUK BELANJA\n";
    cout << "================================\n";
    for (int i = 0; i < jumlahPesanan; i++) {
        cout << left << setw(20) << daftarPesanan[i].nama
             << right << setw(3) << daftarPesanan[i].jumlah << " x "
             << setw(6) << daftarPesanan[i].harga << " = "
             << setw(8) << daftarPesanan[i].subtotal << '\n';
        total += daftarPesanan[i].subtotal;
    }
    cout << "--------------------------------\n";
    cout << "Total Belanja : Rp. " << total << '\n';
    long long uangBayar;
    cout << "Uang Bayar    : Rp. ";
    cin >> uangBayar;
    if (uangBayar >= total) {
        cout << "Kembalian     : Rp. " << uangBayar - total << '\n';
    } else {
        cout << "Kekurangan    : Rp. " << total - uangBayar << '\n';
    }
    cout << "================================\n";
    cout << "Terima kasih!\n";
}

int main() {
    Menu daftarMenu[KAPASITAS_MENU] = {
        {1, "Nasi Goreng", 15000},
        {2, "Mie Goreng", 12000},
        {3, "Kopi Susu", 10000},
        {4, "Es Teh Manis", 4000},
        {5, "Kentang Goreng", 8000}
    };
    int jumlahMenu = 5;
    int kodeBerikutnya = 6;
    int pilihan;

    do {
        cout << "\n================================\n";
        cout << "       POS WARUNG SEDERHANA\n";
        cout << "================================\n";
        cout << "1. Lihat Menu\n";
        cout << "2. Tambah Menu\n";
        cout << "3. Ubah Menu\n";
        cout << "4. Hapus Menu\n";
        cout << "5. Transaksi\n";
        cout << "6. Keluar\n";
        cout << "================================\n";
        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                tampilkanMenu(daftarMenu, jumlahMenu);
                break;
            case 2:
                tambahMenu(daftarMenu, jumlahMenu, kodeBerikutnya);
                break;
            case 3:
                ubahMenu(daftarMenu, jumlahMenu);
                break;
            case 4:
                hapusMenu(daftarMenu, jumlahMenu);
                break;
            case 5:
                transaksi(daftarMenu, jumlahMenu);
                break;
            case 6:
                cout << "Program selesai.\n";
                break;
            default:
                cout << "Pilihan tidak tersedia.\n";
        }
    } while (pilihan != 6);

    return 0;
}
