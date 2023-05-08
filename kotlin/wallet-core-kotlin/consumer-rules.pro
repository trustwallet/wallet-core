-keepclassmembers class com.trustwallet.core.* {
    # Usage example: (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    private long nativeHandle;
    # Usage example: (*env)->GetStaticMethodID(env, class, "createFromNative", "(J)Lcom/trustwallet/core/Account;");
    private static ** createFromNative(long);
}

-keepclassmembers enum com.trustwallet.core.* {
    # Usage example: (*env)->GetFieldID(env, thisClass, "value", "I");
    private int value;
    # Usage example: (*env)->GetMethodID(env, coinClass, "value", "()I");
    public int value();
    # Usage example: (*env)->GetStaticMethodID(env, class, "createFromValue", "(I)Lcom/trustwallet/core/CoinType;");
    public static ** createFromValue(int);
}
