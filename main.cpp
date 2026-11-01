#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
using namespace std;

// ==========================================
// 1. DEFINISI STRUCT & KONSTANTA
// ==========================================

const int KAPASITAS_MENU = 10;
const int KAPASITAS_PESANAN = 10;
const int KAPASITAS_TRANSAKSI = 50;

struct Menu {
    int kode;
    string nama;
    int harga;
    int stok;
};

struct Pesanan {
    int kodeMenu;
    string nama;
    int harga;
    int jumlah;
    long long subtotal;
};

struct Transaksi {
    int id;
    Pesanan item[KAPASITAS_PESANAN];
    int jumlahItem;
    long long total;
    long long bayar;
    long long kembalian;
};

// ==========================================
// 2. FUNGSI PENGELOLAAN MENU
// ==========================================

void tampilkanMenu(const Menu daftarMenu[], int jumlahMenu) {
    cout << "\n============= DAFTAR MENU =============\n";
    if (jumlahMenu == 0) {
        cout << "Belum ada menu.\n";
        return;
    }

    cout << left << setw(8) << "Kode" << setw(22) << "Nama" << setw(14) << "Harga" << "Stok\n";
    cout << "----------------------------------------------------\n";
    for (int i = 0; i < jumlahMenu; i++) {
        cout << left << setw(8) << daftarMenu[i].kode
             << setw(22) << daftarMenu[i].nama
             << "Rp " << setw(11) << daftarMenu[i].harga
             << daftarMenu[i].stok << '\n';
    }
}

int cariMenu(const Menu daftarMenu[], int jumlahMenu, int kode) {
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
    cout << "Masukkan stok awal: ";
    cin >> daftarMenu[jumlahMenu].stok;

    if (daftarMenu[jumlahMenu].harga <= 0 || daftarMenu[jumlahMenu].stok < 0) {
        cout << "Harga harus lebih dari 0 dan stok tidak boleh negatif. Menu gagal ditambahkan.\n";
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

        do {
            cout << "Stok baru (minimal 0): ";
            cin >> daftarMenu[posisi].stok;
            if (daftarMenu[posisi].stok < 0) {
                cout << "Stok tidak boleh negatif.\n";
            }
        } while (daftarMenu[posisi].stok < 0);

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
        for (int i = posisi; i < jumlahMenu - 1; i++) {
            daftarMenu[i] = daftarMenu[i + 1];
        }
        jumlahMenu--;
        cout << "Menu berhasil dihapus.\n";
    }
}

// ==========================================
// 3. FUNGSI TRANSAKSI
// ==========================================

void transaksi(Menu daftarMenu[], int jumlahMenu, Transaksi riwayatTransaksi[], int &jumlahTransaksi) {
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
        } else if (daftarMenu[posisi].stok <= 0) {
            cout << "Maaf, stok menu ini habis.\n";
        } else if (jumlahPesanan >= KAPASITAS_PESANAN) {
            cout << "Kapasitas pesanan sudah penuh.\n";
        } else {
            int indexPesanan = -1;
            for (int i = 0; i < jumlahPesanan; i++) {
                if (daftarPesanan[i].kodeMenu == kode) {
                    indexPesanan = i;
                    break;
                }
            }

            int sudahDipesan = (indexPesanan != -1) ? daftarPesanan[indexPesanan].jumlah : 0;
            int sisaStokTersedia = daftarMenu[posisi].stok - sudahDipesan;

            cout << "Jumlah pembelian (stok tersedia: " << sisaStokTersedia << "): ";
            cin >> jumlah;

            if (jumlah <= 0) {
                cout << "Jumlah harus lebih dari 0. Pesanan tidak ditambahkan.\n";
            } else if (jumlah > sisaStokTersedia) {
                cout << "Stok tidak mencukupi! Maksimal yang bisa dibeli saat ini: " << sisaStokTersedia << ".\n";
            } else {
                if (indexPesanan != -1) {
                    daftarPesanan[indexPesanan].jumlah += jumlah;
                    daftarPesanan[indexPesanan].subtotal += static_cast<long long>(daftarMenu[posisi].harga) * jumlah;
                } else {
                    daftarPesanan[jumlahPesanan].kodeMenu = kode;
                    daftarPesanan[jumlahPesanan].nama = daftarMenu[posisi].nama;
                    daftarPesanan[jumlahPesanan].harga = daftarMenu[posisi].harga;
                    daftarPesanan[jumlahPesanan].jumlah = jumlah;
                    daftarPesanan[jumlahPesanan].subtotal =
                        static_cast<long long>(daftarMenu[posisi].harga) * jumlah;
                    jumlahPesanan++;
                }
                cout << "Pesanan berhasil dicatat.\n";
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
    while (uangBayar < total) {
        cout << "Uang kurang Rp. " << (total - uangBayar) << "! Masukkan ulang uang bayar: Rp. ";
        cin >> uangBayar;
    }

    long long kembalian = uangBayar - total;
    cout << "Kembalian     : Rp. " << kembalian << '\n';
    cout << "================================\n";
    cout << "Terima kasih!\n";

    for (int i = 0; i < jumlahPesanan; i++) {
        int pos = cariMenu(daftarMenu, jumlahMenu, daftarPesanan[i].kodeMenu);
        if (pos != -1) {
            daftarMenu[pos].stok -= daftarPesanan[i].jumlah;
        }
    }

    if (jumlahTransaksi < KAPASITAS_TRANSAKSI) {
        riwayatTransaksi[jumlahTransaksi].id = jumlahTransaksi + 1;
        riwayatTransaksi[jumlahTransaksi].jumlahItem = jumlahPesanan;
        for (int i = 0; i < jumlahPesanan; i++) {
            riwayatTransaksi[jumlahTransaksi].item[i] = daftarPesanan[i];
        }
        riwayatTransaksi[jumlahTransaksi].total = total;
        riwayatTransaksi[jumlahTransaksi].bayar = uangBayar;
        riwayatTransaksi[jumlahTransaksi].kembalian = kembalian;
        jumlahTransaksi++;
    }
}

// ==========================================
// 4. FUNGSI RIWAYAT & REKAP PENJUALAN
// ==========================================

void tampilkanDetailTransaksi(const Transaksi riwayatTransaksi[], int jumlahTransaksi) {
    cout << "\n============= RIWAYAT TRANSAKSI =============\n";
    if (jumlahTransaksi == 0) {
        cout << "Belum ada transaksi yang tercatat.\n";
        return;
    }

    for (int i = 0; i < jumlahTransaksi; i++) {
        cout << "\n----------------------------------------\n";
        cout << "Transaksi #" << riwayatTransaksi[i].id << '\n';
        cout << "----------------------------------------\n";
        for (int j = 0; j < riwayatTransaksi[i].jumlahItem; j++) {
            cout << left << setw(20) << riwayatTransaksi[i].item[j].nama
                 << right << setw(3) << riwayatTransaksi[i].item[j].jumlah << " x "
                 << setw(6) << riwayatTransaksi[i].item[j].harga << " = "
                 << setw(8) << riwayatTransaksi[i].item[j].subtotal << '\n';
        }
        cout << "Total Belanja : Rp. " << riwayatTransaksi[i].total << '\n';
        cout << "Uang Bayar    : Rp. " << riwayatTransaksi[i].bayar << '\n';
        cout << "Kembalian     : Rp. " << riwayatTransaksi[i].kembalian << '\n';
    }
    cout << "========================================\n";
}

void tampilkanRekapPenjualan(const Menu daftarMenu[], int jumlahMenu, const Transaksi riwayatTransaksi[], int jumlahTransaksi) {
    cout << "\n============= REKAP PENJUALAN =============\n";
    if (jumlahTransaksi == 0) {
        cout << "Belum ada transaksi penjualan.\n";
        return;
    }

    long long totalOmset = 0;
    for (int i = 0; i < jumlahTransaksi; i++) {
        totalOmset += riwayatTransaksi[i].total;
    }

    cout << "Total Transaksi Selesai : " << jumlahTransaksi << '\n';
    cout << "Total Omset Penjualan   : Rp. " << totalOmset << "\n\n";

    cout << left << setw(8) << "Kode" << setw(22) << "Nama" << setw(14) << "Terjual" << setw(12) << "Sisa Stok" << "Pendapatan\n";
    cout << "-----------------------------------------------------------------\n";

    for (int i = 0; i < jumlahMenu; i++) {
        int totalTerjual = 0;
        long long totalPendapatan = 0;

        for (int t = 0; t < jumlahTransaksi; t++) {
            for (int p = 0; p < riwayatTransaksi[t].jumlahItem; p++) {
                if (riwayatTransaksi[t].item[p].kodeMenu == daftarMenu[i].kode) {
                    totalTerjual += riwayatTransaksi[t].item[p].jumlah;
                    totalPendapatan += riwayatTransaksi[t].item[p].subtotal;
                }
            }
        }

        cout << left << setw(8) << daftarMenu[i].kode
             << setw(22) << daftarMenu[i].nama
             << right << setw(4) << totalTerjual << " porsi    "
             << setw(6) << daftarMenu[i].stok << "     "
             << left << "Rp. " << totalPendapatan << '\n';
    }
    cout << "-----------------------------------------------------------------\n";
}

// ==========================================
// 5. PROGRAM UTAMA (MAIN)
// ==========================================

int main() {
    Menu daftarMenu[KAPASITAS_MENU] = {
        {1, "Nasi Goreng", 15000, 20},
        {2, "Mie Goreng", 12000, 20},
        {3, "Kopi Susu", 10000, 15},
        {4, "Es Teh Manis", 4000, 30},
        {5, "Kentang Goreng", 8000, 15}
    };
    int jumlahMenu = 5;
    int kodeBerikutnya = 6;

    Transaksi riwayatTransaksi[KAPASITAS_TRANSAKSI];
    int jumlahTransaksi = 0;

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
        cout << "6. Detail Riwayat Transaksi\n";
        cout << "7. Rekap Penjualan\n";
        cout << "8. Keluar\n";
        cout << "================================\n";
        cout << "Pilih menu : ";
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
                transaksi(daftarMenu, jumlahMenu, riwayatTransaksi, jumlahTransaksi);
                break;
            case 6:
                tampilkanDetailTransaksi(riwayatTransaksi, jumlahTransaksi);
                break;
            case 7:
                tampilkanRekapPenjualan(daftarMenu, jumlahMenu, riwayatTransaksi, jumlahTransaksi);
                break;
            case 8:
                cout << "Program selesai.\n";
                break;
            default:
                cout << "Pilihan tidak tersedia.\n";
        }
    } while (pilihan != 8);

    return 0;
}
