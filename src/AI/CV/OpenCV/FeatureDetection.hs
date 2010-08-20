{-# LANGUAGE ForeignFunctionInterface #-}
-- |Feature Detection.
module AI.CV.OpenCV.FeatureDetection (cornerHarris, cornerHarris') where
import Foreign.C.Types (CInt, CDouble)
import Foreign.Ptr (Ptr, castPtr)
import Foreign.Storable (Storable)
import System.IO.Unsafe (unsafePerformIO)
import AI.CV.OpenCV.CxCore
import AI.CV.OpenCV.HIplUtils

foreign import ccall unsafe "opencv/cv.h cvCornerHarris"
  c_cvHarris :: Ptr CvArr -> Ptr CvArr -> CInt -> CInt -> CDouble -> IO ()

harris :: Ptr IplImage -> Ptr IplImage -> Int -> Int -> Double -> IO ()
harris src dst blockSize aperture k = 
    c_cvHarris (castPtr src) (castPtr dst) (fi blockSize) (fi aperture) (rf k)
    where fi = fromIntegral
          rf = realToFrac

-- |Harris corner detector. For each pixel, a 2x2 covariance matrix,
-- @M@, is computed over a @blockSize x blockSize@ neighborhood. The
-- value of @det(M) - 0.04*trace(M)^2@ is stored in the destination
-- image. Corners in the image correspond to local maxima of the
-- destination image. The parameters are the @blockSize@ and the
-- source 'HIplImage'. The Sobel operator used as a preprocessing step
-- is given an aperture size of 3.
cornerHarris :: (ByteOrFloat d, HasDepth d, Storable d) => 
                Int -> HIplImage a MonoChromatic d -> 
                HIplImage FreshImage MonoChromatic Float
cornerHarris blockSize = cornerHarris' blockSize 3 0.04

-- |Harris corner detector. For each pixel, a 2x2 covariance matrix,
-- @M@, is computed over a @blockSize x blockSize@ neighborhood. The
-- value of @det(M) - k*trace(M)^2@ is stored in the destination
-- image. Corners in the image correspond to local maxima of the
-- destination image. The parameters are the @blockSize@, the
-- @aperture@ size to be used by the Sobel operator that is run during
-- corner evaluation, the value of @k@, and the source
-- 'HIplImage'.
cornerHarris' :: (ByteOrFloat d, HasDepth d, Storable d) => 
                 Int -> Int -> Double -> HIplImage a MonoChromatic d -> 
                 HIplImage FreshImage MonoChromatic Float
cornerHarris' blockSize aperture k src = 
    unsafePerformIO $ do dst <- mkHIplImage (width src) (height src)
                         withHIplImage src $ \src' ->
                           withHIplImage dst $ \dst' ->
                             harris src' dst' blockSize aperture k
                         return dst