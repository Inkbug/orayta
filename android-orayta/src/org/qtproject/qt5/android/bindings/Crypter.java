package org.qtproject.qt5.android.bindings;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.security.GeneralSecurityException;
import java.security.MessageDigest;
import java.util.List;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;
import java.util.zip.ZipInputStream;

import javax.crypto.Cipher;
import javax.crypto.CipherInputStream;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.ResolveInfo;
import android.net.Uri;
import android.util.Log;

import org.qtproject.qt5.android.bindings.QtActivity;

//import org.bouncycastle.util.encoders.Base64;

/**
 * Class created for StackOverflow by owlstead. modified by IZAR This is open
 * source, you are free to copy and use for any purpose. usage: encrypt on
 * linux: openssl aes-128-cbc -salt -in password.txt -out password.txt.enc
 */

public class Crypter {

//	private final static int DECRYPT_REQUEST_CODE = 42738492;
	// private final static String ZF_NAME = "zf_name";
	// private final static String INT_FILE = "int_file";
	// private final static String TARGET = "target";
//	private final static String METHODNAME = "methodName";
	// private final static String DECRYPT = "decrypt";
//	private final static String DECRYPT_REQUEST_METHODNAME = "get decrypt";
	// private final static String ERROR= "error";
	public final static int KUKAYTA_NOT_INSTALLED = -1;
//	private static final Charset UTF8 = Charset.forName("UTF-8");
	private static final int INDEX_KEY = 0;
	private static final int INDEX_IV = 1;
	private static final int ITERATIONS = 1;

//	private static final int ARG_INDEX_FILENAME = 0;
//	private static final int ARG_INDEX_PASSWORD = 1;

	private static final int SALT_OFFSET = 8;
	private static final int SALT_SIZE = 8;
	private static final int CIPHERTEXT_OFFSET = SALT_OFFSET + SALT_SIZE;

	private static final int KEY_SIZE_BITS = 128;

	/**
	 * 
	 * @return 1 for success, 2 for 'not needed', 0 for fail, or
	 *         KUKAYTA_NOT_INSTALLED
	 */
	public static int decryptRequest() {
                QtActivity me = org.qtproject.qt5.android.bindings.QtActivity/**/.getActivity();
                if (isKukaytaInstalled() >= 2)
			return 2;
		me.doServiceConnect();
		return 1;
	}

	public static int zipDecrypt(String zipFilename, String intFile,
			String target, String one) {
                QtActivity me = org.qtproject.qt5.android.bindings.QtActivity.getActivity();
		Log.d("IZAR", "zip decrypt request");
		if (me == null)
			return -3;

		if (me.getZfn()==null)
			return KUKAYTA_NOT_INSTALLED;

		String zfn = me.getZfn();

		
		return zipDecrypt2(zipFilename, intFile, target, one, zfn);

		// return 0; // failed
	}

	public static void installKukatya(Activity me) {
		try {
			Uri uri = Uri.parse("market://details?id=org.Orayta.kukayta.paid");
			Intent intent = new Intent(Intent.ACTION_VIEW, uri);
			me.startActivity(intent);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public static void installKukatya() {
                QtActivity me = org.qtproject.qt5.android.bindings.QtActivity.getActivity();
		installKukatya(me);
	}

	



	/**
	 * Thanks go to Ola Bini for releasing this source on his blog. The source
	 * was obtained from <a
	 * href="http://olabini.com/blog/tag/evp_bytestokey/">here</a> .
	 */
	private static byte[][] EVP_BytesToKey(int key_len, int iv_len,
			MessageDigest md, byte[] salt, byte[] data, int count) {
		byte[][] both = new byte[2][];
		byte[] key = new byte[key_len];
		int key_ix = 0;
		byte[] iv = new byte[iv_len];
		int iv_ix = 0;
		both[0] = key;
		both[1] = iv;
		byte[] md_buf = null;
		int nkey = key_len;
		int niv = iv_len;
		int i = 0;
		if (data == null) {
			return both;
		}
		int addmd = 0;
		for (;;) {
			md.reset();
			if (addmd++ > 0) {
				md.update(md_buf);
			}
			md.update(data);
			if (null != salt) {
				md.update(salt, 0, 8);
			}
			md_buf = md.digest();
			for (i = 1; i < count; i++) {
				md.reset();
				md.update(md_buf);
				md_buf = md.digest();
			}
			i = 0;
			if (nkey > 0) {
				for (;;) {
					if (nkey == 0)
						break;
					if (i == md_buf.length)
						break;
					key[key_ix++] = md_buf[i];
					nkey--;
					i++;
				}
			}
			if (niv > 0 && i != md_buf.length) {
				for (;;) {
					if (niv == 0)
						break;
					if (i == md_buf.length)
						break;
					iv[iv_ix++] = md_buf[i];
					niv--;
					i++;
				}
			}
			if (nkey == 0 && niv == 0) {
				break;
			}
		}
		for (i = 0; i < md_buf.length; i++) {
			md_buf[i] = 0;
		}
		return both;
	}

	public static InputStream decrypt(String filename, String pwd) {
		File f = new File(filename);
		try {
			byte[] db = fileToBytes(f.getAbsolutePath());
			// System.out.println("file size: "+ f.length());
			return decrypt(db, pwd);
		} catch (IOException e) {
			throw new IllegalStateException(e);
		}
	}

	public static InputStream decrypt(byte[] db, String pwd) {
		// Log.d("IZAR", "in decrypt");
		// System.out.println("to decrypt\n"+new String(db));

		try {
			// --- read base 64 encoded file ---

			// byte[] headerSaltAndCipherText = Base64.decode(db /*,
			// Base64.DEFAULT*/);
			byte[] headerSaltAndCipherText = db;

			// --- extract salt & encrypted ---

			// header is "Salted__", ASCII encoded, if salt is being used (the
			// default)
			byte[] salt = copyOfRange(headerSaltAndCipherText, SALT_OFFSET,
					SALT_OFFSET + SALT_SIZE);
			byte[] encrypted = copyOfRange(headerSaltAndCipherText,
					CIPHERTEXT_OFFSET, headerSaltAndCipherText.length);

			// --- specify cipher and digest for EVP_BytesToKey method ---

			Cipher aesCBC = Cipher.getInstance("AES/CBC/PKCS5Padding");
			MessageDigest md5 = MessageDigest.getInstance("MD5");

			// --- create key and IV ---

			// the IV is useless, OpenSSL might as well have use zero's
			final byte[][] keyAndIV = EVP_BytesToKey(KEY_SIZE_BITS / Byte.SIZE,
					aesCBC.getBlockSize(), md5, salt, pwd.getBytes(),
					ITERATIONS);
			SecretKeySpec key = new SecretKeySpec(keyAndIV[INDEX_KEY], "AES");
			IvParameterSpec iv = new IvParameterSpec(keyAndIV[INDEX_IV]);

			// --- initialize cipher instance and decrypt ---

			aesCBC.init(Cipher.DECRYPT_MODE, key, iv);
			//TODO -
			// byte[] decrypted = aesCBC.doFinal(encrypted);
			InputStream decrypted = new CipherInputStream(
					new ByteArrayInputStream(encrypted), aesCBC);
			return decrypted;

		} catch (GeneralSecurityException e) {
			throw new IllegalStateException(e);
		}
	}

	public static int zipDecrypt2(String zipFilename, String intFile,
			String target, String one, String three) {
		// Log.d("IZAR", "in zipDecrypt");
		ZipInputStream zis = null;
		try {
			zis = new ZipInputStream(new FileInputStream(zipFilename));
			ZipFile zf = new ZipFile(zipFilename);
			ZipEntry ze;
			byte[] fileBytes;
			// byte[] buffer= new byte[2048];

			while ((ze = zis.getNextEntry()) != null) {
				InputStream is = zf.getInputStream(ze);
				BufferedInputStream bis = new BufferedInputStream(is);
				if (ze.getName().equals(intFile)) {
					System.out.println("ze size: " + ze.getSize());
					fileBytes = new byte[(int) ze.getSize()];
					bis.read(fileBytes);
					// is.read(fileBytes);
					bis.close();
					zis.closeEntry();

					zis.close();
					InputStream decrypted = decrypt(fileBytes, one
							+ "H%e5oO!4!Y" + three);

					
					 BufferedOutputStream bos = new BufferedOutputStream(new
					 FileOutputStream(target));
					byte[] buff = new byte[1024];
					int read = 0;
					while ((read = decrypted.read(buff)) != -1) {
						bos.write(buff, 0, read);
					}
					decrypted.close();
					bos.close();
					return 1; // success
					
					
				}

			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				zis.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}

		return 0; // error occurred

	}

	private static byte[] fileToBytes(String filename) throws IOException {
		Log.d("IZAR", "in fileToBytes");
		File file = new File(filename);
		byte[] bytes = new byte[(int) file.length()];
		FileInputStream fis = new FileInputStream(file);

		fis.read(bytes);
		fis.close();
		return bytes;
	}

//	private static void bytesToFile(byte[] bytes, String filename)
//			throws IOException {
//		Log.d("IZAR", "in bytesToFile");
//		FileOutputStream fos = new FileOutputStream(filename);
//		fos.write(bytes);
//		fos.close();
//
//	}

	public static byte[] copyOfRange(byte[] original, int from, int to) {
		Log.d("IZAR", "in copyOfRange");
		int newLength = to - from;
		if (newLength < 0)
			throw new IllegalArgumentException(from + " > " + to);
		byte[] copy = new byte[newLength];
		System.arraycopy(original, from, copy, 0,
				Math.min(original.length - from, newLength));
		return copy;
	}
	
	/**
	 * 
	 * @return 2 if already have passwd, 1 if found kukayta apk installed, 0 if not found, -1 on error.
	 */
	public static int isKukaytaInstalled(){
		Log.d("IZAR", "in iskukins()");
//		try {
//			QtActivity me = org.kde.necessitas.origo.QtActivity.getActivity();
//			if (me.getZfn()==null || me.getZfn().equals(""))
//				return 0;
//			return 1;
//		} catch (Exception e) {
//			e.printStackTrace();
//			return -1;
//		}
		try {
                        QtActivity me = org.qtproject.qt5.android.bindings.QtActivity/**/.getActivity();
			if (!(me.getZfn()==null || me.getZfn().equals("")))
				return 2;
			Intent remote = new Intent("org.Orayta.kukayta.paid.DeCrypter"); 
			List<ResolveInfo> list = me.getPackageManager().queryIntentServices(remote, 0);
			boolean isIntentSafe = list.size() > 0;
			if (!isIntentSafe) 
				return 0;
			
			}
			catch(Exception e) {
				
				return -1;
			}
		return 1;
	}
	
	public static int testInt(){
		return 1;
	}

}
