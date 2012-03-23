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
	}
	
	
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