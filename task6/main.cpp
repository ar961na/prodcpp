#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <sstream>

// Абстрактный класс CheckPoint
class CheckPoint {
public:
    virtual ~CheckPoint() = default;

    virtual std::string getName() const = 0;

    virtual double getLatitude() const = 0;

    virtual double getLongitude() const = 0;

    virtual std::string getInfo() const = 0;

    virtual bool isMandatory() const = 0;

    virtual double getPenalty() const = 0;
};

// Конкретный класс MandatoryCheckPoint (обязательный КП)
class MandatoryCheckPoint : public CheckPoint {
private:
    std::string name;
    double latitude;
    double longitude;

public:
    MandatoryCheckPoint(const std::string &name, double latitude, double longitude)
            : name(name), latitude(latitude), longitude(longitude) {}

    std::string getName() const override { return name; }

    double getLatitude() const override { return latitude; }

    double getLongitude() const override { return longitude; }

    std::string getInfo() const override { return "незачёт СУ"; }

    bool isMandatory() const override { return true; }

    double getPenalty() const override { return 0.0; }
};

// Конкретный класс OptionalCheckPoint (необязательный КП)
class OptionalCheckPoint : public CheckPoint {
private:
    std::string name;
    double latitude;
    double longitude;
    double penalty;

public:
    OptionalCheckPoint(const std::string &name, double latitude, double longitude, double penalty)
            : name(name), latitude(latitude), longitude(longitude), penalty(penalty) {}

    std::string getName() const override { return name; }

    double getLatitude() const override { return latitude; }

    double getLongitude() const override { return longitude; }

    std::string getInfo() const override {
        std::ostringstream oss;
        oss << "Штраф: " << penalty << " ч.";
        return oss.str();
    }

    bool isMandatory() const override { return false; }

    double getPenalty() const override { return penalty; }
};

// Класс Builder для создания КП
class CheckPointBuilder {
public:
    virtual ~CheckPointBuilder() = default;

    virtual std::shared_ptr<CheckPoint> build() = 0;
};

// Конкретный Builder для обязательного КП
class MandatoryCheckPointBuilder : public CheckPointBuilder {
private:
    std::string name;
    double latitude;
    double longitude;

public:
    MandatoryCheckPointBuilder &setName(const std::string &name) {
        this->name = name;
        return *this;
    }

    MandatoryCheckPointBuilder &setLatitude(double latitude) {
        this->latitude = latitude;
        return *this;
    }

    MandatoryCheckPointBuilder &setLongitude(double longitude) {
        this->longitude = longitude;
        return *this;
    }

    std::shared_ptr<CheckPoint> build() override {
        return std::make_shared<MandatoryCheckPoint>(name, latitude, longitude);
    }
};

// Конкретный Builder для необязательного КП
class OptionalCheckPointBuilder : public CheckPointBuilder {
private:
    std::string name;
    double latitude;
    double longitude;
    double penalty;

public:
    OptionalCheckPointBuilder &setName(const std::string &name) {
        this->name = name;
        return *this;
    }

    OptionalCheckPointBuilder &setLatitude(double latitude) {
        this->latitude = latitude;
        return *this;
    }

    OptionalCheckPointBuilder &setLongitude(double longitude) {
        this->longitude = longitude;
        return *this;
    }

    OptionalCheckPointBuilder &setPenalty(double penalty) {
        this->penalty = penalty;
        return *this;
    }

    std::shared_ptr<CheckPoint> build() override {
        return std::make_shared<OptionalCheckPoint>(name, latitude, longitude, penalty);
    }
};

// Класс для обработки списка КП
class CheckPointListProcessor {
private:
    std::vector<std::shared_ptr<CheckPoint>> checkPoints;

public:
    void addCheckPoint(const std::shared_ptr<CheckPoint> &cp) {
        checkPoints.push_back(cp);
    }

    void printCheckPoints() const {
        int index = 1;
        for (const auto &cp: checkPoints) {
            std::cout << index++ << ". Имя: " << cp->getName() << ", Координаты: ("
                      << cp->getLatitude() << ", " << cp->getLongitude() << "), "
                      << cp->getInfo() << std::endl;
        }
    }

    double calculateTotalPenalty() const {
        double totalPenalty = 0.0;
        for (const auto &cp: checkPoints) {
            if (!cp->isMandatory()) {
                totalPenalty += cp->getPenalty();
            }
        }
        return totalPenalty;
    }
};

// Пример использования
int main() {
    CheckPointListProcessor processor;

    MandatoryCheckPointBuilder mandatoryBuilder;
    processor.addCheckPoint(mandatoryBuilder.setName("КП1").setLatitude(45.0).setLongitude(90.0).build());
    processor.addCheckPoint(mandatoryBuilder.setName("КП2").setLatitude(46.0).setLongitude(91.0).build());

    OptionalCheckPointBuilder optionalBuilder;
    processor.addCheckPoint(
            optionalBuilder.setName("КП3").setLatitude(47.0).setLongitude(92.0).setPenalty(1.5).build());
    processor.addCheckPoint(
            optionalBuilder.setName("КП4").setLatitude(48.0).setLongitude(93.0).setPenalty(2.0).build());

    std::cout << "Список КП:" << std::endl;
    processor.printCheckPoints();

    double totalPenalty = processor.calculateTotalPenalty();
    std::cout << "Суммарный штраф: " << totalPenalty << " ч." << std::endl;

    return 0;
}
