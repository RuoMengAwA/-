package com.example.btdemo.bean;

public class ReportBean {

    private String id;
    private Object vehicleIdEncrypted;
    private String vehicleId;
    private String description;
    private String dateTime;
    private String image;
    private int dangerousLevel;

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public Object getVehicleIdEncrypted() {
        return vehicleIdEncrypted;
    }

    public void setVehicleIdEncrypted(Object vehicleIdEncrypted) {
        this.vehicleIdEncrypted = vehicleIdEncrypted;
    }

    public String getVehicleId() {
        return vehicleId;
    }

    public void setVehicleId(String vehicleId) {
        this.vehicleId = vehicleId;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public String getDateTime() {
        return dateTime;
    }

    public void setDateTime(String dateTime) {
        this.dateTime = dateTime;
    }

    public String getImage() {
        return image;
    }

    public void setImage(String image) {
        this.image = image;
    }

    public int getDangerousLevel() {
        return dangerousLevel;
    }

    public void setDangerousLevel(int dangerousLevel) {
        this.dangerousLevel = dangerousLevel;
    }
}
