- The difference between this and the last "finance" submission, that the last one do not check whether the password is 8 characters or not ..etc, but this one actually check password in register.html

    ## Specification

    - Creating database tables


    This is the functions i used to create the database tables

    db.execute("""
        CREATE TABLE IF NOT EXISTS users_stocks
            (
            user_stock_id INTEGER NOT NULL,
            user_id INTEGER NOT NULL,
            stock_id INTEGER NOT NULL,
            stock_symbol TEXT NOT NULL,
            shares INTEGER NOT NULL,
            PRIMARY KEY (user_stock_id),
            FOREIGN KEY (user_id) REFERENCES users(id),
            FOREIGN KEY (stock_id) REFERENCES stocks(stock_id)
            )
            """)

    db.execute("""
        CREATE TABLE IF NOT EXISTS stocks
            (
            stock_id INTEGER NOT NULL,
            stock_symbol TEXT NOT NULL UNIQUE,
            PRIMARY KEY (stock_id)
            )
            """)

    db.execute("""
        CREATE TABLE IF NOT EXISTS transactions
            (
            transaction_id INTEGER NOT NULL,
            user_id INTEGER NOT NULL,
            quote_id INTEGER NOT NULL,
            transaction_date NUMERIC NOT NULL,
            quote_symbol TEXT NOT NULL,
            price_per_share NUMERIC NOT NULL,
            shares INTEGER NOT NULL,
            holding_value NUMERIC NOT NULL,
            trading_fees NUMERIC NOT NULL,
            total_amount INTEGER NOT NULL,
            transaction_type TEXT NOT NULL,
            PRIMARY KEY (transaction_id),
            FOREIGN KEY (user_id) REFERENCES users(id),
            FOREIGN KEY (quote_id) REFERENCES stocks(stock_id)
            )
            """)

    # create_indexes_on_tables
    queries = [
        "CREATE UNIQUE INDEX IF NOT EXISTS users_stocks_user_id_stock_symbol_uindex ON users_stocks (user_id, stock_symbol)",
        "CREATE INDEX IF NOT EXISTS users_stocks_user_id_index ON users_stocks (user_id)",
        "CREATE INDEX IF NOT EXISTS users_stocks_stock_id_index ON users_stocks (stock_id)",
        "CREATE UNIQUE INDEX IF NOT EXISTS stocks_stock_symbol_uindex ON stocks (stock_symbol)",
        "CREATE INDEX IF NOT EXISTS transactions_user_id_index ON transactions (user_id)",
        "CREATE INDEX IF NOT EXISTS transactions_quote_id_index ON transactions (quote_id)",
        "CREATE INDEX IF NOT EXISTS transactions_user_id_transaction_type_index ON transactions (user_id, transaction_type)"
    ]

    for query in queries:
        Boolean_value = db.execute(query)