package com.example.demo.Models;

public class WebSocketResponseEntity {
    private String deviceSerial;
    private String responseType;
    private String responseContent;

    public WebSocketResponseEntity(String deviceSerial, String responseType, String responseContent) {
        this.deviceSerial = deviceSerial;
        this.responseType = responseType;
        this.responseContent = responseContent;
    }

    public String getDeviceSerial() {
        return deviceSerial;
    }

    public void setDeviceSerial(String deviceSerial) {
        this.deviceSerial = deviceSerial;
    }

    public String getResponseType() {
        return responseType;
    }

    public void setResponseType(String responseType) {
        this.responseType = responseType;
    }

    public String getResponseContent() {
        return responseContent;
    }

    public void setResponseContent(String responseContent) {
        this.responseContent = responseContent;
    }
}
