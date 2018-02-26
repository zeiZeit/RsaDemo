// IOpenAtci.aidl
package com.wind.emode.atci;

// Declare any non-default types here with import statements

interface IOpenAtci {
    /**
     * Demonstrates some basic types that you can use as parameters
     * and return values in AIDL.
     */
    String getCode();

    String returnCode(String code);
}
