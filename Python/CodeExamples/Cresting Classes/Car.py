# Defining the Class Car
class Car:
    def __init__(self, model, color):
        self.model = model
        self.color = color
    
    def displayCar(self):
        print(self.model)
        print(self.color)