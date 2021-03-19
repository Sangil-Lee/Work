class Car:
    model = ""
    color = ""


car = Car()

car.model = "Volvo"
car.color = "Blue"

print(car.color + " " + car.model)


car.model = "Ford"
car.color = "Green"

print(car.color + " " + car.model)