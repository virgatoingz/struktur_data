#include <iostream>
#include <mysql/mysql.h>
#include <sstream>
#include <limits>

using namespace std;

const char* hostname = "127.0.0.1";
const char* user = "root";
const char* pass = "123";
const char* dbname = "uas_db";
unsigned int port = 31234;
const char* unixsocket = NULL;
unsigned long clientflag = 0;

MYSQL* connect_db() {
    MYSQL* conn = mysql_init(0);
    if (conn) {
        conn = mysql_real_connect(conn, hostname, user, pass, dbname, port, unixsocket, clientflag);
        if (conn) {
            cout << "Connected to the database successfully." << endl;
        } else {
            cerr << "Connection failed: " << mysql_error(conn) << endl;
        }
    } else {
        cerr << "mysql_init failed" << endl;
    }
    return conn;
}

bool login(string username, string password, string &role)
{
    MYSQL *conn = connect_db();
    if (conn)
    {
        string query = "SELECT role FROM users WHERE username = '" + username + "' AND password = '" + password + "'";
        if (mysql_query(conn, query.c_str()) == 0)
        {
            MYSQL_RES *res = mysql_store_result(conn);
            if (mysql_num_rows(res) > 0)
            {
                MYSQL_ROW row = mysql_fetch_row(res);
                role = row[0];
                cout << "Login success." << endl;
                mysql_free_result(res);
                mysql_close(conn);
                return true;
            }
            else
            {
                cout << "Login failed." << endl;
                mysql_free_result(res);
                mysql_close(conn);
                return false;
            }
        }
        return false;
    }
    return false;
}

bool mendaftar(string username, string password, string role = "user")
{
    MYSQL *conn = connect_db();
    if (conn)
    {
        string query = "INSERT INTO users (username, password, role) VALUES ('" + username + "', '" + password + "', '" + role + "')";
        if (mysql_query(conn, query.c_str()))
        {
            cerr << "INSERT failed: " << mysql_error(conn) << endl;
        }
       else if (role == "admin")
        {
            cout << "Admin successfully added." << endl;
        }
        else
        {
            cout << "User successfully added." << endl;
        }
        mysql_close(conn);
    }
    return true;
}

void create_buku(const string& judul_buku, const string& nama_author, const string& penerbit, const string& tahun_terbit,const string& isbn , const string& genre, const string& jumlah) {
    MYSQL* conn = connect_db();
    if (conn) {
        string query = "INSERT INTO tb_buku (judul_buku, nama_author, penerbit, tahun_terbit, isbn,genre,jumlah) VALUES ('" + judul_buku + "', '" + nama_author + "', '" + penerbit + "', '" + tahun_terbit + "', '" + isbn + "', '" + genre + "', '" + jumlah + "')";
        if (mysql_query(conn, query.c_str())) {
            cerr << "INSERT failed: " << mysql_error(conn) << endl;
        } else {
            cout << "Buku successfully added." << endl;
        }
        mysql_close(conn);
    }
}

void get_buku() {
    MYSQL* conn = connect_db();
    if (conn) {
        if (mysql_query(conn, "SELECT * FROM tb_buku")) {
            cerr << "SELECT failed: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            cerr << "mysql_store_result failed: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            cout << "ID: " << row[0] << ", Judul Buku: " << row[1] << ", Nama Author: " << row[2] << ", Penerbit: " << row[3] << ", Tahun Terbit: " << row[4] << ", ISBN: " << row[5] << ", Genre: " << row[6] << ", Jumlah: " << row[7] << endl;
        }

        mysql_free_result(res);
        mysql_close(conn);
    }
}

void update_buku(int buku_id, const string& judul_buku, const string& nama_author, const string& penerbit, const string& tahun_terbit, const string& isbn, const string& genre, const string& jumlah) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "UPDATE tb_buku SET judul_buku = '" << judul_buku << "', nama_author = '" << nama_author << "', penerbit = '" << penerbit << "', tahun_terbit = '" << tahun_terbit << "', isbn = '" << isbn  << "', genre = '" << genre  << "', jumlah = '" << jumlah<< "' WHERE id = " << buku_id;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "UPDATE failed: " << mysql_error(conn) << endl;
        } else {
            cout << "Buku successfully updated." << endl;
        }
        mysql_close(conn);
    }
}

void delete_buku(int buku_id) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream query;
        query << "DELETE FROM tb_buku WHERE id = " << buku_id;
        if (mysql_query(conn, query.str().c_str())) {
            cerr << "DELETE failed: " << mysql_error(conn) << endl;
        } else {
            cout << "Buku successfully deleted." << endl;
        }
        mysql_close(conn);
    }
}

void tampilanadmin()
{
    int choice;
    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Tambah Buku\n";
        cout << "2. Tampilkan Buku\n";
        cout << "3. Update Buku\n";
        cout << "4. Delete Buku\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            string judul_buku, nama_author, penerbit, tahun_terbit, isbn,genre,jumlah;
            cout << "Enter Judul Buku: ";
            cin.ignore();
            getline(cin, judul_buku);
            cout << "Enter Nama Author: ";
            getline(cin, nama_author);
            cout << "Enter Penerbit: ";
            getline(cin, penerbit);
            cout << "Enter Tahun Terbit: ";
            getline(cin, tahun_terbit);
            cout << "Enter ISBN: ";
            getline(cin, isbn);
            cout << "Enter Genre: ";
            getline(cin, genre);
            cout << "Enter jumlah: ";
            getline(cin, jumlah);
            create_buku(judul_buku, nama_author, penerbit, tahun_terbit, isbn,genre,jumlah);
        } else if (choice == 2) {
            get_buku();
        } else if (choice == 3) {
            int buku_id;
            string judul_buku, nama_author, penerbit, tahun_terbit, isbn,genre,jumlah;
            cout << "Enter user ID to update: ";
            cin >> buku_id;
            cin.ignore();
            cout << "Enter new Judul Buku: ";
            getline(cin, judul_buku);
            cout << "Enter new Nama Author: ";
            getline(cin, nama_author);
            cout << "Enter new Penerbit: ";
            getline(cin, penerbit);
            cout << "Enter new Tahun Terbit: ";
            getline(cin, tahun_terbit);
            cout << "Enter new ISBN: ";
            getline(cin, isbn);
            cout << "Enter new Genre: ";
            getline(cin, genre);
            cout << "Enter new Jumlah: ";
            getline(cin, jumlah);
            update_buku(buku_id, judul_buku, nama_author, penerbit, tahun_terbit, isbn,genre,jumlah);
        } else if (choice == 4) {
            int buku_id;
            cout << "Enter Buku ID to Delete: ";
            cin >> buku_id;
            delete_buku(buku_id);
        } else if (choice == 5) {
            break;
        } else {
            cout << "Salah Memasukan Pilihan." << endl;
        }
    }
}

void tampilanuser()
{
    while (true)
    {
        int choice;
        cout << "1. Tampilkan Buku" << endl;
        cout << "2. Exit" << endl;
        cout << "Choice: ";
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Salah Memasukan Pilihan." << endl;
        }
        if (choice == 2)
        {
            cout << "Exiting..." << endl;
            return;
        }
        switch (choice)
        {
        case 1:
        {
            get_buku();
            break;
        }
        case 2:
            cout << "Exiting..." << endl;
            return;
            break;
        default:
            cout << "Salah Memasukan Pilihan." << endl;
            break;
        }
    }
}

void login(){
    string username, password, role;
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;
    if (login(username, password, role))
    {
        cout << "Selamat Datang " << username << "!" << endl;
        if (role == "admin")
        {
            cout << "Hallo Admin." << endl;
            tampilanadmin();
        }
        else
        {
            cout << "Anda Login Sebagai Pengunjung" << endl;
            tampilanuser();
        }
    }
    else
    {
        cout << "Login Gagal." << endl;
    }
}

int main() {
    int choice;
    do
    {
        string username, password;
        cout << "Selamat Datang di Perpustakaan" << endl;
        cout << "1. Resgister User" << endl;
        cout << "2. Login" << endl;
        cout << "3. Exit" << endl;
        // int choice;
        cout << "Choice: ";
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Salah Memasukan Pilihan" << endl;
        }

        switch (choice)
        {
        case 1:
            cout << "Username: ";
            cin >> username;
            cout << "Password: ";
            cin >> password;
            mendaftar(username, password);
            break;
        case 2:
            login();
            break;
        case 3:
            break;
        default:
            cout << "Salah Memasukan Pilihan" << endl;
            break;
        }
    } while (choice != 3);

    return 0;
}