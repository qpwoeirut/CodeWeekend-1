import json

TESTS = 50

tests = [
    json.loads(open(f'inputs/{str(i + 1).zfill(3)}.json').read()) for i in range(TESTS)
]

for i, test in enumerate(tests):
    print(i + 1)
    # print(test["hero"])
    # print(test["start_x"], test["start_y"])
    print(test["width"], test["height"])
    # print(test["num_turns"])

    total_exp = sum(map(lambda m: m["exp"], test["monsters"]))
    print(len(test["monsters"]), total_exp)

    level_cost = 0
    for level in range(1, 10000):
        incremental_cost = 1000 + level * (level - 1) * 50
        level_cost += incremental_cost
        if total_exp < level_cost:
            print("Max level:", level - 1)
            break
    print()

# for i, test in enumerate(tests):
#     print(i, test["width"] * test["height"] * len(test["monsters"]))

print(max(map(lambda t: t["width"], tests)))
print(max(map(lambda t: t["height"], tests)))
