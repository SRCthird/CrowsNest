#ifndef APP_SERVICE_H
#define APP_SERVICE_H

#include <crow.h>
#include <crow/app.h>
#include <crow/http_response.h>
#include <crow/mustache.h>
#include <crow/utility.h>
#include <soci/soci.h>
#include <string>
#include <stdexcept>

#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>
#include <string>
#include <stdexcept>
#include <mysql/mysql.h>

struct User {
    int id;
    std::string name;

    User() : id(0), name("") {}
    User(int id, std::string name) : id(id), name(name) {}
};

class database_service {
private:
  soci::session& sql;

public:
    database_service(soci::session& sql) : sql(sql) {}

    void create(const User& data) {
        sql << "INSERT INTO {{ table }} ({{ values }}) VALUES ({{ value_identifiers }})", soci::use(data.id), soci::use(data.name);
    }

    User read(int id) {
        User user;
        soci::indicator ind;
        sql << "SELECT id, name FROM users WHERE id = :id", soci::use(id), into(user.id, ind), into(user.name, ind);

        if (ind == soci::i_null) {
            throw std::runtime_error("User not found");
        }

        return user;
    }

    void update(const User& user) {
        sql << "UPDATE users SET name = :name WHERE id = :id", soci::use(user.name), soci::use(user.id);
    }

    void remove(int id) {
        sql << "DELETE FROM users WHERE id = :id", soci::use(id);
    }
};

/**
int main() {
    try {
        soci::session sql(soci::mysql, "host=localhost db=mydb user=myuser password=mypassword");

        UserRepository userRepository(sql);

        // Create a new user
        User newUser(1, "John Doe");
        userRepository.create(newUser);

        // Read the user
        User user = userRepository.read(1);
        std::cout << "User: " << user.id << ", " << user.name << std::endl;

        // Update the user's name
        user.name = "Jane Doe";
        userRepository.update(user);

        // Delete the user
        userRepository.remove(user.id);
    } catch (const soci::soci_error& e) {
        std::cerr << "SOCI error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
*/

#endif // APP_SERVICE_H
