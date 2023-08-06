package com.Source.S1_MCC.MCC.Trans;

public class ObjSelected {

    private int mID;
    private String mDescription;
    private boolean mIsWaitConfirm;
    private String mPassString;
    private String mUserInputValue;
    private boolean mUserInputCancel;
    private boolean mIsCompleted;
    private boolean mIsCancelled;
    private int mObjType;
    private boolean mIsContinue;
    private int mSelectedPosition;
    private boolean mIsInputValue;
    private String mUserInputHeader;
    private boolean mIsPressed;
    private boolean mIsItemExceedLimit;

    public ObjSelected(int ID, String description, boolean IsWaitConfirm, String passString, String userInputValue, boolean userInputCancel, int selectedPosition, boolean isInputValue, String userInputHeader) {
        mID = ID;
        mDescription = description;
        mIsWaitConfirm = IsWaitConfirm;
        mPassString = passString;
        mUserInputValue = userInputValue;
        mUserInputCancel = userInputCancel;
        mSelectedPosition = selectedPosition;
        mIsInputValue = isInputValue;
        mUserInputHeader = userInputHeader;
    }

    public int getID() {
        return mID;
    }

    public void setID(int ID) {
        mID = ID;
    }

    public String getDescription() {
        return mDescription;
    }

    public void setDescription(String description) {
        mDescription = description;
    }

    public boolean isWaitConfirm() {
        return mIsWaitConfirm;
    }

    public void setWaitConfirm(boolean waitConfirm) {
        mIsWaitConfirm = waitConfirm;
    }

    public String getPassString() {
        return mPassString;
    }

    public void setPassString(String passString) {
        mPassString = passString;
    }

    public String getUserInputValue() {
        return mUserInputValue;
    }

    public void setUserInputValue(String userInputValue) {
        mUserInputValue = userInputValue;
    }

    public boolean isUserInputCancel() {
        return mUserInputCancel;
    }

    public void setUserInputCancel(boolean userInputCancel) {
        mUserInputCancel = userInputCancel;
    }

    public boolean isCompleted() {
        return mIsCompleted;
    }

    public void setCompleted(boolean completed) {
        mIsCompleted = completed;
    }

    public boolean isCancelled() {
        return mIsCancelled;
    }

    public void setCancelled(boolean cancelled) {
        this.mIsCancelled = cancelled;
    }

    public int getObjType() {
        return mObjType;
    }

    public void setObjType(int objType) {
        mObjType = objType;
    }

    public boolean isContinue() {
        return mIsContinue;
    }

    public void setContinue(boolean aContinue) {
        mIsContinue = aContinue;
    }

    public int getSelectedPosition() {
        return mSelectedPosition;
    }

    public void setSelectedPosition(int selectedPosition) {
        mSelectedPosition = selectedPosition;
    }

    public boolean isInputValue() {
        return mIsInputValue;
    }

    public void setInputValue(boolean inputValue) {
        mIsInputValue = inputValue;
    }

    public String getmUserInputHeader() {
        return mUserInputHeader;
    }

    public void setmUserInputHeader(String mUserInputHeader) {
        this.mUserInputHeader = mUserInputHeader;
    }

    public boolean isIsPressed() {
        return mIsPressed;
    }

    public void setIsPressed(boolean mIsPressed) {
        this.mIsPressed = mIsPressed;
    }

    public boolean ismIsItemExceedLimit() {
        return mIsItemExceedLimit;
    }

    public void setmIsItemExceedLimit(boolean mIsItemLimit) {
        this.mIsItemExceedLimit = mIsItemLimit;
    }
}
