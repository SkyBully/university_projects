package controllers;

import database.Database;
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

public class CtrlSearchPlane {
    private ObservableList<Plane> planes = FXCollections.observableArrayList();
    private  CtrlAddFly parent;
    @FXML
    public TextField planeName;
    @FXML
    public TextField aircraftRegistration;
    @FXML
    public TableView <Plane> tablePlane;
    @FXML
    public TableColumn <Plane, String> columnId;
    @FXML
    public TableColumn <Plane, String> columnPlaneName;
    @FXML
    public TableColumn <Plane, String> columnAircraftRegistration;

    public CtrlAddFly getParent() {
        return parent;
    }

    public void setParent(CtrlAddFly parent) {
        this.parent = parent;
    }

    public void searchPlane(ActionEvent actionEvent) throws SQLException {
        tablePlane.getItems().clear();
        String name = planeName.getText();
        String  airRegistration = aircraftRegistration.getText();
        Database db = new Database();
        ResultSet result = db.getData("public.plane","name_plane LIKE '" + name + "%' AND aircraft_registration LIKE '" + airRegistration + "%'");
        while(result.next()){
            planes.add(new Plane(result.getString("id_plane"),result.getString("name_plane"), result.getString("aircraft_registration")));
            // nazov = result.getString("name_plane");
            // mnozstvo = result.getInt("aircraft_registration");

        }
        //planes.add(new Plane("1","Boing 737", "AR345343"));
       //planes.add(new Plane("2","Boing 757", "BL233443"));

        columnId.setCellValueFactory(new PropertyValueFactory<Plane, String>("id"));
        columnPlaneName.setCellValueFactory(new PropertyValueFactory<Plane, String>("name"));
        columnAircraftRegistration.setCellValueFactory(new PropertyValueFactory<Plane, String>("aircraftRegistration"));

        tablePlane.setItems(planes);

    }

    public void applyPlane(ActionEvent actionEvent) {
        //System.out.println(tablePlane.getSelectionModel().getSelectedItem().getId());
        parent.setPlaneTextField(tablePlane.getSelectionModel().getSelectedItem().getId());
        Stage stage = (Stage) planeName.getScene().getWindow();
        stage.close();
}

}