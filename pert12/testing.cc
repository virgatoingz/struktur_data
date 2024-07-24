#include <iostream>
#include <mysql/mysql.h>
using namespace std;

const char *hostname = "127.0.0.1";
const char *user = "root";
const char *pass = "1234";
const char *dbname = "toko";
unsigned int port = 31234;
const char *unixsocket = NULL;
unsigned long clientflag = 0;

MYSQL *connect_db()
{
    MYSQL *conn = mysql_init(0);
    if (conn)
    {
        conn = mysql_real_connect(conn, hostname, user, pass, dbname, port, unixsocket, clientflag);
        if (conn)
        {
            cout << "Connected to the database successfully." << endl;
        }
        else
        {
            cerr << "Connection failed: " << mysql_error(conn) << endl;
        }
    }
    else
    {
        cerr << "mysql_init failed" << endl;
    }
    return conn;
}

bool register_user(string username, string password, string role = "user")
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

bool login(string username, string password, string &userRole)
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
                userRole = row[0];
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

void create_barang(string nama_barang, int jumlah_barang, string harga_barang)
{
    MYSQL *conn = connect_db();
    if (conn)
    {
        string query = "INSERT INTO barang (nama_barang, jumlah_barang, harga_barang) VALUES ('" + nama_barang + "', '" + to_string(jumlah_barang) + "', '" + harga_barang + "')";
        if (mysql_query(conn, query.c_str()))
        {
            cerr << "INSERT failed: " << mysql_error(conn) << endl;
        }
        else
        {
            cout << "Barang successfully added." << endl;
        }
        mysql_close(conn);
    }
}

void get_barang()
{
    MYSQL *conn = connect_db();
    if (conn)
    {
        string query = "SELECT * FROM barang";
        if (mysql_query(conn, query.c_str()))
        {
            cerr << "SELECT failed: " << mysql_error(conn) << endl;
        }
        else
        {
            MYSQL_RES *res = mysql_store_result(conn);
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != NULL)
            {
                cout << "ID: " << row[0] << "\nNama Barang: " << row[1] << "\nJumlah: " << row[2] << "\nHarga: " << row[3] << endl;
            }
            mysql_free_result(res);
        }
        mysql_close(conn);
    }
}

void update_barang(int id, string nama_barang, int jumlah_barang, double harga_barang)
{
    MYSQL *conn = connect_db();
    if (conn)
    {
        string query = "UPDATE barang SET nama_barang='" + nama_barang + "', jumlah_barang=" + to_string(jumlah_barang) + ", harga_barang=" + to_string(harga_barang) + " WHERE id=" + to_string(id);
        if (mysql_query(conn, query.c_str()))
        {
            cerr << "UPDATE failed: " << mysql_error(conn) << endl;
        }
        else
        {
            cout << "Barang updated successfully." << endl;
        }
        mysql_close(conn);
    }
}

void delete_barang(int id)
{
    MYSQL *conn = connect_db();
    if (conn)
    {
        string query = "DELETE FROM barang WHERE id=" + to_string(id);
        if (mysql_query(conn, query.c_str()))
        {
            cerr << "DELETE failed: " << mysql_error(conn) << endl;
        }
        else
        {
            cout << "Barang deleted successfully." << endl;
        }
        mysql_close(conn);
    }
}

void display_admin_menu()
{
    while (true)
    {
        int choice;
        cout << "1. Create Barang" << endl;
        cout << "2. Get Barang" << endl;
        cout << "3. Update Barang" << endl;
        cout << "4. Delete Barang" << endl;
        cout << "5. Exit" << endl;
        cout << "Choice: ";
        cin >> choice;

        if (choice == 5)
        {
            cout << "Exiting..." << endl;
            return;
        }

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice." << endl;
        }
        switch (choice)
        {
        case 1:
        {
            string nama_barang, harga_barang;
            int jumlah_barang;
            cout << "Nama Barang: ";
            cin >> nama_barang;
            cout << "Jumlah Barang: ";
            cin >> jumlah_barang;
            cout << "Harga Barang: ";
            cin >> harga_barang;
            create_barang(nama_barang, jumlah_barang, harga_barang);
            break;
        }
        case 2:
        {
            get_barang();
            break;
        }
        case 3:
        {
            int id, jumlah_barang;
            string nama_barang;
            double harga_barang;
            cout << "ID: ";
            cin >> id;
            cout << "Nama Barang: ";
            cin >> nama_barang;
            cout << "Jumlah Barang: ";
            cin >> jumlah_barang;
            cout << "Harga Barang: ";
            cin >> harga_barang;
            update_barang(id, nama_barang, jumlah_barang, harga_barang);
            break;
        }
        case 4:
        {
            int id;
            cout << "ID: ";
            cin >> id;
            delete_barang(id);
            break;
        }
        case 5:
            cout << "Exiting..." << endl;
            return;
            break;
        default:
            cout << "Invalid choice." << endl;
            break;
        }
    }
}

void display_user_menu()
{
    while (true)
    {
        int choice;
        cout << "1. Get Barang" << endl;
        cout << "2. Exit" << endl;
        cout << "Choice: ";
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice." << endl;
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
            get_barang();
            break;
        }
        case 2:
            cout << "Exiting..." << endl;
            return;
            break;
        default:
            cout << "Invalid choice." << endl;
            break;
        }
    }
}

void main_login()
{
    string username, password, userRole;
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;
    if (login(username, password, userRole))
    {
        cout << "Welcome, " << username << "!" << endl;
        if (userRole == "admin")
        {
            cout << "You are an admin." << endl;
            display_admin_menu();
        }
        else
        {
            cout << "You are a user." << endl;
            display_user_menu();
        }
    }
    else
    {
        cout << "Login failed." << endl;
    }
}

int main()
{
    int choice;
    do
    {
        string username, password;
        cout << "Welcome to Toko App" << endl;
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "3. Exit" << endl;
        // int choice;
        cout << "Choice: ";
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice." << endl;
        }

        switch (choice)
        {
        case 1:
            cout << "Username: ";
            cin >> username;
            cout << "Password: ";
            cin >> password;
            register_user(username, password);
            break;
        case 2:
            main_login();
            break;
        case 3:
            break;
        default:
            cout << "Invalid choice." << endl;
            break;
        }
    } while (choice != 3);

    return 0;
}