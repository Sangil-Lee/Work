#!/usr/bin/python

import torch
import torch.nn as nn
import torch.optim as optim
import numpy as np
import matplotlib.pyplot as plt

# 1. 데이터 시뮬레이션 (가상의 센서 데이터)
def create_time_series(length=1000, trend=0.1, seasonality=10, noise=5):
    time = np.arange(length)
    series = trend * time + seasonality * np.sin(time / 10) + noise * np.random.randn(length)
    return series.astype(np.float32)

series = create_time_series()

# 2. 데이터 전처리
def preprocess_data(series, n_steps=5):
    X = []
    y = []
    for i in range(len(series) - n_steps):
        X.append(series[i : i + n_steps])
        y.append(series[i + n_steps])
    return np.array(X), np.array(y)

n_steps = 10
X, y = preprocess_data(series, n_steps)

# 데이터를 PyTorch 텐서로 변환
#X_tensor = torch.tensor(X.reshape(X.shape, n_steps, 1))
X_tensor = torch.tensor(X.reshape(X.shape[0], n_steps, 1))
y_tensor = torch.tensor(y.reshape(-1, 1))

# 학습 및 테스트 데이터 분리
train_size = int(0.8 * len(X))
X_train = X_tensor[:train_size]
y_train = y_tensor[:train_size]
X_test = X_tensor[train_size:]
y_test = y_tensor[train_size:]

# 3. 예측 모델 정의 (간단한 LSTM)
class TimeSeriesPredictor(nn.Module):
    def __init__(self, input_size=1, hidden_size=50, output_size=1):
        super(TimeSeriesPredictor, self).__init__()
        self.lstm = nn.LSTM(input_size, hidden_size, batch_first=True)
        self.linear = nn.Linear(hidden_size, output_size)

    def forward(self, x):
        out, _ = self.lstm(x)
        out = self.linear(out[:, -1, :]) # 마지막 시퀀스 값만 사용
        return out

model = TimeSeriesPredictor()
criterion = nn.MSELoss()
optimizer = optim.Adam(model.parameters(), lr=0.001)

# 4. 모델 학습
epochs = 100
for epoch in range(epochs):
    optimizer.zero_grad()
    outputs = model(X_train)
    loss = criterion(outputs, y_train)
    loss.backward()
    optimizer.step()
    if (epoch + 1) % 10 == 0:
        print(f'Epoch [{epoch+1}/{epochs}], Loss: {loss.item():.4f}')

# 5. 예측 및 알람 시스템 구현
model.eval()
with torch.no_grad():
    test_outputs = model(X_test)
    predicted = test_outputs.numpy()
    actual = y_test.numpy()

# 6. 알람 임계값 설정 및 알람 발생 로직
alarm_threshold = np.mean(actual) + 2 * np.std(actual) # 실제 값의 평균 + 2 표준편차를 임계값으로 설정
alarms = predicted > alarm_threshold

# 7. 결과 시각화
plt.figure(figsize=(12, 6))
plt.plot(np.arange(len(series)), series, label='Original Series')
plt.plot(np.arange(train_size + n_steps, len(series)), actual, label='Actual Values', color='green')
plt.plot(np.arange(train_size + n_steps, len(series)), predicted, label='Predicted Values', color='red')
plt.scatter(np.arange(train_size + n_steps, len(series))[alarms.flatten()], predicted[alarms].flatten(), color='orange', marker='o', label='Alarm Triggered')
plt.axhline(y=alarm_threshold, color='r', linestyle='--', label='Alarm Threshold')
plt.xlabel('Time')
plt.ylabel('Sensor Reading')
plt.title('Time Series Prediction with Alarm System')
plt.legend()
plt.grid(True)
plt.show()

print(f"\nAlarm Threshold: {alarm_threshold:.2f}")
print(f"Number of Alarms Triggered: {np.sum(alarms)}")
