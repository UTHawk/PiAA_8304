import matplotlib.pyplot as plt

points_x = []
points_y = []

for i in range(2, 41):
    with open(f"{i}.txt") as input_file:
        points_x.append(i)
        points_y.append(int(input_file.readline()) / 1000)

points_x, points_y = zip(*sorted(zip(points_x, points_y)))

fig, axs = plt.subplots()
axs.scatter(points_x, points_y)
plt.grid()
plt.show()
