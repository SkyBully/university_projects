package controllers;

import database.BoardingPass;
import database.Database;
import database.Flight;
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

public class CtrlSearchBoardingPass {
    private ObservableList<BoardingPass> info = FXCollections.observableArrayList();
    @FXML
    public TextField boardingPass;
    @FXML
    public TableView<BoardingPass> tableInfo;
    @FXML
    public TableColumn<BoardingPass, String> columnId;
    @FXML
    public TableColumn <BoardingPass, String> columnFirstName;
    @FXML
    public TableColumn <BoardingPass, String> columnLastName;
    @FXML
    public TableColumn <BoardingPass, String> columnPassportID;
    @FXML
    public TableColumn <BoardingPass, String> columnEmail;
    @FXML
    public TableColumn <BoardingPass, String> columnPhoneNumber;

    private  CtrlEditBaggage parent;
    private String price;

    public void searchBoardingPass(ActionEvent actionEvent) throws SQLException {
        tableInfo.getItems().clear();
        String boardingPassText = boardingPass.getText();

        Database db = new Database();
        ResultSet result = db.getData("public.boardingpass bp JOIN public.passenger p ON bp.id_passenger=p.id_passenger","id_boarding_pass = " + boardingPassText);
        while(result.next()){
            info.add(new BoardingPass(result.getString("id_boarding_pass"),result.getString("first_name"), result.getString("last_name"),result.getString("passport_id"),result.getString("email"),result.getString("phone_number")));
            // nazov = result.getString("name_plane");
            // mnozstvo = result.getInt("aircraft_registration");
            price = result.getString("price") + ",00 EUR";
        }
        //planes.add(new Plane("1","Boing 737", "AR345343"));
        //planes.add(new Plane("2","Boing 757", "BL233443"));

        columnId.setCellValueFactory(new PropertyValueFactory<BoardingPass, String>("id"));
        columnFirstName.setCellValueFactory(new PropertyValueFactory<BoardingPass, String>("firstName"));
        columnLastName.setCellValueFactory(new PropertyValueFactory<BoardingPass, String>("lastName"));
        columnPassportID.setCellValueFactory(new PropertyValueFactory<BoardingPass, String>("passportID"));
        columnEmail.setCellValueFactory(new PropertyValueFactory<BoardingPass, String>("email"));
        columnPhoneNumber.setCellValueFactory(new PropertyValueFactory<BoardingPass, String>("phoneNumber"));
        tableInfo.setItems(info);
    }

    public void apply(ActionEvent actionEvent) {
        parent.setIdBoardingPassTextField(boardingPass.getText());
        parent.setPriceTextField(price);
        Stage stage = (Stage) boardingPass.getScene().getWindow();
        stage.close();
    }

    public void setParent(CtrlEditBaggage parent) {
        this.parent = parent;
    }
}
