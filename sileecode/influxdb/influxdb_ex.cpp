#include <influxdb.hpp>
#include <string>
#include <vector>
#include <iostream>

template<typename T>
class InfluxDBHandler {
public:
    InfluxDBHandler(const std::string& db_name, const std::string& measurement)
        : db_name_(db_name), measurement_(measurement) {}

    void saveData(const std::vector<T>& data) {
        influxdb_cpp::server_info si("127.0.0.1", 8086, db_name_);
        for (const auto& value : data) {
            std::string resp;
            influxdb_cpp::builder()
                .meas(measurement_)
                .field("value", value)
                .post_http(si, &resp);
        }
    }

    std::vector<T> fetchData() {
        influxdb_cpp::server_info si("127.0.0.1", 8086, db_name_);
        std::string query = "SELECT value FROM " + measurement_;
        std::string resp;
        influxdb_cpp::query(resp, query, si);

        std::vector<T> result;
        // Parsing response and extracting values (assuming JSON format)
        // This is a simplified example, actual implementation may vary
        size_t pos = 0;
        while ((pos = resp.find("value")) != std::string::npos) {
            size_t start = resp.find(":", pos) + 1;
            size_t end = resp.find(",", start);
            T value = std::stod(resp.substr(start, end - start));
            result.push_back(value);
            resp = resp.substr(end);
        }
        return result;
    }

private:
    std::string db_name_;
    std::string measurement_;
};

int main() {
    // InfluxDBHandler 사용 예제
    InfluxDBHandler<double> influxHandler("example_db", "matrix_data");

    // 데이터 저장
    std::vector<double> data_to_save = {1.1, 2.2, 3.3, 4.4, 5.5};
    influxHandler.saveData(data_to_save);

    // 데이터 추출
    std::vector<double> fetched_data = influxHandler.fetchData();
    std::cout << "Fetched data from InfluxDB:" << std::endl;
    for (const auto& value : fetched_data) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    return 0;
}
//g++ -I/home/ctrluser/Test/influxdb/influxdb-cpp -o test influxdb_ex.cpp
