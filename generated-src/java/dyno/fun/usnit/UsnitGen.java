// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from usnit.djinni

package dyno.fun.usnit;

import java.util.concurrent.atomic.AtomicBoolean;

public abstract class UsnitGen {
    public abstract boolean initialize(String conf, LangType lang, UsnitEventGen callback);

    public abstract boolean save();

    public abstract void setType(UsnitType type);

    public abstract void setInput(float value);

    public abstract String getResult(UsnitType type);

    public abstract String getUnitName(UsnitType type);

    public abstract UsnitType getLongType();

    public abstract UsnitType getMassType();

    public abstract UsnitType getSquareType();

    public abstract UsnitType getVolumeType();

    public static native UsnitGen instance();

    private static final class CppProxy extends UsnitGen
    {
        private final long nativeRef;
        private final AtomicBoolean destroyed = new AtomicBoolean(false);

        private CppProxy(long nativeRef)
        {
            if (nativeRef == 0) throw new RuntimeException("nativeRef is zero");
            this.nativeRef = nativeRef;
        }

        private native void nativeDestroy(long nativeRef);
        public void destroy()
        {
            boolean destroyed = this.destroyed.getAndSet(true);
            if (!destroyed) nativeDestroy(this.nativeRef);
        }
        protected void finalize() throws java.lang.Throwable
        {
            destroy();
            super.finalize();
        }

        @Override
        public boolean initialize(String conf, LangType lang, UsnitEventGen callback)
        {
            assert !this.destroyed.get() : "trying to use a destroyed object";
            return native_initialize(this.nativeRef, conf, lang, callback);
        }
        private native boolean native_initialize(long _nativeRef, String conf, LangType lang, UsnitEventGen callback);

        @Override
        public boolean save()
        {
            assert !this.destroyed.get() : "trying to use a destroyed object";
            return native_save(this.nativeRef);
        }
        private native boolean native_save(long _nativeRef);

        @Override
        public void setType(UsnitType type)
        {
            assert !this.destroyed.get() : "trying to use a destroyed object";
            native_setType(this.nativeRef, type);
        }
        private native void native_setType(long _nativeRef, UsnitType type);

        @Override
        public void setInput(float value)
        {
            assert !this.destroyed.get() : "trying to use a destroyed object";
            native_setInput(this.nativeRef, value);
        }
        private native void native_setInput(long _nativeRef, float value);

        @Override
        public String getResult(UsnitType type)
        {
            assert !this.destroyed.get() : "trying to use a destroyed object";
            return native_getResult(this.nativeRef, type);
        }
        private native String native_getResult(long _nativeRef, UsnitType type);

        @Override
        public String getUnitName(UsnitType type)
        {
            assert !this.destroyed.get() : "trying to use a destroyed object";
            return native_getUnitName(this.nativeRef, type);
        }
        private native String native_getUnitName(long _nativeRef, UsnitType type);

        @Override
        public UsnitType getLongType()
        {
            assert !this.destroyed.get() : "trying to use a destroyed object";
            return native_getLongType(this.nativeRef);
        }
        private native UsnitType native_getLongType(long _nativeRef);

        @Override
        public UsnitType getMassType()
        {
            assert !this.destroyed.get() : "trying to use a destroyed object";
            return native_getMassType(this.nativeRef);
        }
        private native UsnitType native_getMassType(long _nativeRef);

        @Override
        public UsnitType getSquareType()
        {
            assert !this.destroyed.get() : "trying to use a destroyed object";
            return native_getSquareType(this.nativeRef);
        }
        private native UsnitType native_getSquareType(long _nativeRef);

        @Override
        public UsnitType getVolumeType()
        {
            assert !this.destroyed.get() : "trying to use a destroyed object";
            return native_getVolumeType(this.nativeRef);
        }
        private native UsnitType native_getVolumeType(long _nativeRef);
    }
}