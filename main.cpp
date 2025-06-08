#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

bool giveChange(int change, map<int, int> &cashCount, vector<int> &changeGiven)
{
    vector<int> denominations;

    for (const auto &pair : cashCount)
    {
        denominations.push_back(pair.first);
    }

    sort(denominations.begin(), denominations.end(), greater<int>());

    for (int denom : denominations)
    {
        while (change >= denom && cashCount[denom] > 0)
        {
            change -= denom;
            changeGiven.push_back(denom);
            cashCount[denom]--;
        }
        if (change == 0)
            break;
    }

    return change == 0;
}

int main()
{
    map<string, int> barang = {
        {"cocacola", 10000},
        {"sprite", 12000},
        {"fanta", 8000},
        {"aqua", 5000},
        {"pocari", 7000},
        {"tehbotol", 6000},
        {"indomilk", 7000},
    };

    map<string, int> stokBarang = {
        {"cocacola", 3},
        {"sprite", 2},
        {"fanta", 1},
        {"aqua", 2},
        {"pocari", 1},
        {"tehbotol", 1},
        {"indomilk", 1},
    };

    map<int, int> cashCount = {
        {50000, 1},
        {20000, 1},
        {10000, 1},
        {5000, 2},
        {2000, 3},
        {1000, 2},
    };

    vector<int> pecahanDiterima = {1000, 2000, 5000, 10000, 20000, 50000};

    while (true)
    {
        cout << "\n=== DAFTAR BARANG ===" << endl;

        vector<string> namaBarang;
        int index = 1;

        for (const auto &item : barang)
        {
            cout << index << ". " << item.first << ": Rp" << item.second;
            cout << " (Stok: " << stokBarang[item.first] << ")" << endl;
            namaBarang.push_back(item.first);
            index++;
        }

        int pilihan;
        cout << "\nPilih barang (masukkan nomor): ";
        cin >> pilihan;

        if (pilihan < 1 || pilihan > barang.size())
        {
            cout << "Pilihan tidak valid. Kembali ke menu." << endl;
            continue;
        }

        string namaBarangPilihan = namaBarang[pilihan - 1];
        int price = barang[namaBarangPilihan];

        if (stokBarang[namaBarangPilihan] <= 0)
        {
            cout << "Stok barang " << namaBarangPilihan << " habis. Kembali ke menu." << endl;
            continue;
        }

        int paid = 0;
        vector<int> uangDariUser;

        cout << "Harga " << namaBarangPilihan << ": Rp" << price << endl;
        cout << "Masukkan uang satu per satu (ketik 0 jika ingin membatalkan):" << endl;

        cout << "\nMesin menerima pecahan: ";
        for (int pecahan : pecahanDiterima)
        {
            cout << "Rp" << pecahan << " ";
        }
        cout << endl;

        while (paid < price)
        {
            char cancel;
            int uang;
            cout << "Masukkan uang: ";
            cin >> uang;

            if (uang == 0)
            {
                cout << "Transaksi dibatalkan. Uang Anda dikembalikan." << endl;
                for (int u : uangDariUser)
                {
                    cashCount[u]--;
                }
                paid = 0;
                break;
            }

            if (find(pecahanDiterima.begin(), pecahanDiterima.end(), uang) != pecahanDiterima.end())
            {

                paid += uang;
                cashCount[uang]++;            // Uang masuk ke mesin
                uangDariUser.push_back(uang); // Catat untuk rollback jika perlu
                cout << "Uang Rp" << uang << " diterima. Total: Rp" << paid << endl;
            }
            else
            {
                cout << "Pecahan tidak diterima mesin. Masukkan pecahan lain." << endl;
            }
        }

        if (paid == 0)

            continue;

        int change = paid - price;
        vector<int> changeGiven;
        map<int, int> tempCash = cashCount;

        bool bisaKembali = giveChange(change, tempCash, changeGiven);

        if (!bisaKembali)
        {
            cout << "Maaf, mesin tidak bisa memberikan kembalian yang sesuai." << endl;
            cout << "Transaksi dibatalkan. Uang Anda dikembalikan." << endl;

            // Rollback: keluarkan uang yang tadi dimasukkan user
            for (int uang : uangDariUser)
            {
                cashCount[uang]--; // keluarkan dari mesin
            }
            continue;
        }

        // Transaksi berhasil
        cashCount = tempCash; // update mesin setelah kembalian diberikan
        stokBarang[namaBarangPilihan]--;

        cout << "Anda membeli: " << namaBarangPilihan << " seharga Rp" << price << endl;

        if (changeGiven.empty())
        {
            cout << "Tidak ada kembalian." << endl;
        }
        else
        {
            cout << "Kembalian Anda: ";
            for (int uang : changeGiven)
            {
                cout << "Rp" << uang << " ";
            }
            cout << endl;
        }

        cout << "Terima kasih telah berbelanja!" << endl;

        cout << "\n=== STOK UANG KEMBALIAN SEKARANG ===" << endl;
        for (auto it = cashCount.rbegin(); it != cashCount.rend(); ++it)
        {
            cout << "Rp" << it->first << " : " << it->second << " lembar" << endl;
        }
    }

    return 0;
}