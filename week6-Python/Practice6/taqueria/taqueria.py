def main():
    menue = {
    "Baja Taco": 4.00,
    "Burrito": 7.50,
    "Bowl": 8.50,
    "Nachos": 11.00,
    "Quesadilla": 8.50,
    "Super Burrito": 8.50,
    "Super Quesadilla": 9.50,
    "Taco": 3.00,
    "Tortilla Salad": 8.00
    }
    order = []
    total_cost = 0

    while True:
        try:
            item = input('Enter an item :')
            item = item.lower().title()
            if item in menue:
                order.append(item)
                item_price = menue[item]
                total_cost += item_price
                print(f'Total Cost: ${total_cost:.2f}')
        except EOFError :
            if len(order) == 0:
                print('No items were entered !')
                return
            else:
                print()
                return





main()