package controllers;

import database.Database;
import database.Flight;
import database.Plane;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.TextField;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.stage.Stage;

import java.sql.ResultSet;
import java.sql.SQLException;

public class CtrlSearchFlight {
    private ObservableList<Flight> flights = FXCollections.observableArrayList();
    private  CtrlAddFly parent;
    @FXML
    public TextField from;
    @FXML
    public TextField to;
    @FXML
    public TextField time;
    @FXML
    public TableView<Flight> tablePlane;
    @FXML
    public TableColumn<Flight, String> columnId;
    @FXML
    public TableColumn <Flight, String> columnFrom;
    @FXML
    public TableColumn <Flight, String> columnTo;
    @FXML
    public TableColumn <Flight, String> columnTime;

    public CtrlAddFly getParent() {
        return parent;
    }

    public void setParent(CtrlAddFly parent) {
        this.parent = parent;
    }

    public void searchFlight(ActionEvent actionEvent) throws SQLException {
        tablePlane.getItems().clear();
        String fromText = from.getText();
        String  toText = to.getText();
        Database db = new Database();
        ResultSet result = db.getData("public.flight","\"from\" LIKE '" + fromText + "%' AND \"to\" LIKE '" + toText + "%'");
        while(result.next()){
            flights.add(new Flight(result.getString("id_flight"),result.getString("from"), result.getString("to"),result.getString("time")));
            // nazov = result.getString("name_plane");
            // mnozstvo = result.getInt("aircraft_registration");

        }
        //planes.add(new Plane("1","Boing 737", "AR345343"));
        //planes.add(new Plane("2","Boing 757", "BL233443"));

        columnId.setCellValueFactory(new PropertyValueFactory<Flight, String>("id"));
        columnFrom.setCellValueFactory(new PropertyValueFactory<Flight, String>("from"));
        columnTo.setCellValueFactory(new PropertyValueFactory<Flight, String>("to"));
        columnTime.setCellValueFactory(new PropertyValueFactory<Flight, String>("time"));

        tablePlane.setItems(flights);
    }

    public void applyFlight(ActionEvent actionEvent) {
        parent.setFlightTextField(tablePlane.getSelectionModel().getSelectedItem().getId());
        Stage stage = (Stage) from.getScene().getWindow();
        stage.close();
    }
}
