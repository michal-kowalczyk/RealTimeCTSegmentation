package pl.ekk.mkk.necessitas;

import android.hardware.Camera;
import android.hardware.Camera.PreviewCallback;


public class CameraSupport{
	protected Camera camera;
	protected final FrameContainer frame;
	
	class FrameContainer{
		private byte[] frame;
		
		public byte[] getFrame(){
			return frame;
		}
		
		public void setFrame (byte[] frame){
			this.frame = frame;
		}
	};
	
	
	public CameraSupport(){
		frame = new FrameContainer();

		//System.load("/data/local/ImaGene/libhello-jni.so");///sdcard/libhello-jni.so
		//android.util.Log.e("CameraSupport", stringFromJNI());//loadLibrary
        //System.load("/sdcard/libtwolib-second.so");///data/local/qt_aux/libffmpeg.so
	}
	
    /* A native method that is implemented by the
     * 'hello-jni' native library, which is packaged
     * with this application.
     */
    //public native String  stringFromJNI();

    /* This is another native method declaration that is *not*
     * implemented by 'hello-jni'. This is simply to show that
     * you can declare as many native methods in your Java code
     * as you want, their implementation is searched in the
     * currently loaded native libraries only the first time
     * you call them.
     *
     * Trying to call this function will result in a
     * java.lang.UnsatisfiedLinkError exception !
     */
    //public native String  unimplementedStringFromJNI();

    /* this is used to load the 'hello-jni' library on application
     * startup. The library has already been unpacked into
     * /data/data/com.example.HelloJni/lib/libhello-jni.so at
     * installation time by the package manager.
     */
    /*static {
        System.loadLibrary("/sdcard/libhello-jni.so");
    }*/
	
//    public native int add (int x, int y);
	
	
	public void startPreview (int width, int height){
		camera = Camera.open();
		Camera.Parameters parameters = camera.getParameters();
		int bitsPerPixel = android.graphics.ImageFormat.getBitsPerPixel(parameters.getPreviewFormat());
        
		parameters.setPreviewSize(width, height);
		camera.setParameters(parameters);
		
        camera.addCallbackBuffer(new byte[((width * height) * bitsPerPixel) / 8 + 1]);
        camera.addCallbackBuffer(new byte[((width * height) * bitsPerPixel) / 8 + 1]);
        camera.addCallbackBuffer(new byte[((width * height) * bitsPerPixel) / 8 + 1]);
		camera.setPreviewCallbackWithBuffer(new PreviewCallback(){
			public void onPreviewFrame(byte[] data, Camera arg1) {
				frame.setFrame(data);
                camera.addCallbackBuffer(data);
			}
		});
		
		camera.startPreview();
	}
	
	
	public void stopPreview (){
		camera.stopPreview();
		camera.setPreviewCallbackWithBuffer(null);
		camera.release();
		camera = null;
	}
    
	
	public byte[] getFrame (){
		try{
            byte[] result = frame.getFrame();
			frame.setFrame(null);
			return result;
		} catch (Exception e){
			return null;
		}
	}
}