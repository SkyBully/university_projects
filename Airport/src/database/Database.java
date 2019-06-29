package database;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.Arrays;
import java.util.HashMap;
import java.util.LinkedList;

public class Database {

    static final String DB_URL = "jdbc:postgresql://147.175.179.9:5432/postgres";
    //static final String DB_URL = "jdbc:postgresql://localhost:5432/postgres";
    static final String USER = "user_robert";
    //static final String USER = "postgres";
    static final String PASS = "123321";
    //static final String PASS = "322144";
    private Connection connection = null;
    public void addData(String table, String columns, String values) {
        System.out.println("Testing connection to PostgreSQL JDBC");

        try {
            Class.forName("org.postgresql.Driver");
        } catch (ClassNotFoundException e) {
            System.out.println("PostgreSQL JDBC Driver is not found. Include it in your library path ");
            e.printStackTrace();
            return;
        }

        System.out.println("PostgreSQL JDBC Driver successfully connected");

        try {
            connection = DriverManager
                    .getConnection(DB_URL, USER, PASS);
            Statement stmt = connection.createStatement();
                //System.out.println("lala ");
                    String query = "INSERT INTO " + table + " " + columns + " VALUES (" + values + ")";
            //System.out.println(query);
                    stmt.executeUpdate(query);

        } catch (SQLException e) {
            System.out.println("Connection Failed");
            e.printStackTrace();
            return;
        }

        if (connection != null) {
            System.out.println("You successfully connected to database and added data");
        } else {
            System.out.println("Failed to make connection to database");
        }
    }

    public ResultSet getData(String table, String where) {
        System.out.println("Testing connection to PostgreSQL JDBC");

        try {
            Class.forName("org.postgresql.Driver");
        } catch (ClassNotFoundException e) {
            System.out.println("PostgreSQL JDBC Driver is not found. Include it in your library path ");
            e.printStackTrace();
            return null;
        }

        System.out.println("PostgreSQL JDBC Driver successfully connected");

        try {
            connection = DriverManager
                    .getConnection(DB_URL, USER, PASS);
            Statement stmt = connection.createStatement();
            //System.out.println("lala ");
            String query = "SELECT * FROM " + table + " WHERE " + where;
            //System.out.println(query);
            ResultSet result = stmt.executeQuery(query);
            return result;

        } catch (SQLException e) {
            System.out.println("Connection Failed");
            e.printStackTrace();
            return null;
        }

        /*if (connection != null) {
            System.out.println("You successfully connected to database and added data");
        } else {
            System.out.println("Failed to make connection to database");
        }*/
    }

    public void addDataTransaction(String table, String columns, String values, String transaction) {
        System.out.println("Testing connection to PostgreSQL JDBC");

        try {
            Class.forName("org.postgresql.Driver");
        } catch (ClassNotFoundException e) {
            System.out.println("PostgreSQL JDBC Driver is not found. Include it in your library path ");
            e.printStackTrace();
            return;
        }

        System.out.println("PostgreSQL JDBC Driver successfully connected");

        try {
            connection = DriverManager
                    .getConnection(DB_URL, USER, PASS);
            Statement stmt = connection.createStatement();
            //System.out.println("lala ");
            String query = "BEGIN; INSERT INTO " + table + " " + columns + " VALUES (" + values + ") " + transaction + "; COMMIT;";
            //System.out.println(query);
            stmt.executeUpdate(query);

        } catch (SQLException e) {
            System.out.println("Connection Failed");
            e.printStackTrace();
            return;
        }

        if (connection != null) {
            System.out.println("You successfully connected to database and added data");
        } else {
            System.out.println("Failed to make connection to database");
        }
    }

    public void deleteDataTransaction(String table, String columns, String values, String transaction) {
        System.out.println("Testing connection to PostgreSQL JDBC");

        try {
            Class.forName("org.postgresql.Driver");
        } catch (ClassNotFoundException e) {
            System.out.println("PostgreSQL JDBC Driver is not found. Include it in your library path ");
            e.printStackTrace();
            return;
        }

        System.out.println("PostgreSQL JDBC Driver successfully connected");

        try {
            connection = DriverManager
                    .getConnection(DB_URL, USER, PASS);
            Statement stmt = connection.createStatement();
            //System.out.println("lala ");
            String query = "BEGIN; DELETE FROM " + table + " WHERE " + columns + "= " + values + "; " + transaction + "; COMMIT;";
            //System.out.println(query);
            stmt.executeUpdate(query);

        } catch (SQLException e) {
            System.out.println("Connection Failed");
            e.printStackTrace();
            return;
        }

        if (connection != null) {
            System.out.println("You successfully connected to database and added data");
        } else {
            System.out.println("Failed to make connection to database");
        }
    }

}
