<?php
/**
* @file sendMail.php
*/

/*
* php ini settings
*/
ini_set( 'date.timezone', 'Europe/Helsinki' );

/*
* setting up autoloader
*/

$currentFilePath = dirname(realpath(__FILE__));
set_include_path($currentFilePath . '/library/' . PATH_SEPARATOR . get_include_path());
require_once 'Zend/Loader/StandardAutoloader.php';
$loader = new \Zend\Loader\StandardAutoloader();
$loader->registerNamespace('Zend', './library/Zend');
$loader->register();

/*
* mail settings used
*/

$host = 'gmail.com';
$smtpServer = 'smtp.gmail.com';
$port = '587';
$connectionClass = 'login';
$username = 'kutyamaci17@gmail.com';
$password = 'tancoskurva';

$connectionConfig = array(
	'username' => $username,
	'password' => $password,
	'ssl'      => 'tls'
);

$connectionOptions = array(
    'name'              => $host,
    'host'              => $smtpServer,
	'port'              => $port,
    'connection_class'  => $connectionClass,
    'connection_config' => $connectionConfig
);

$fromAddress = 'kutyamaci17@gmail.com';
$fromName = 'Kutya Maci';
$toAdresses = array(
	array(
		'address' => 'duplamatyi@gmail.com',
		'name' => 'Matyas Matyas'
	),
	array(
		'address' => 'istvan0315@yahoo.com',
		'name' => 'Hevele Istvan'
	),
		array(
		'address' => 'sscrazyy@gmail.com',
		'name' => 'Szasz Robert'
	),
	array(
		'address' => 'boldi.istvan@gmail.com',
		'name' => 'Boldi Istvan'
	)
);

$attachments = array(
	array(
		'source' => './test',
		'destination' => './test.zip'
	),
	array(
		'source' => './test2',
		'destination' => './test2.zip'
	)
);



/*
* used Zend namespaces
*/
use Zend\Mail\Message;
use Zend\Mail\Transport\Smtp as SmtpTransport;
use Zend\Mail\Transport\SmtpOptions;
use Zend\Mime\Message as MimeMessage;
use Zend\Mime\Part as MimePart;

$transport = new SmtpTransport();

$options   = new SmtpOptions($connectionOptions);

$message = new Zend\Mail\Message();

$subject = 'Szevasz Ecsem!';
$body = '<p>Ez jobb mint a lamafarokszor.</p>';
$files = array();

foreach ($attachments as $attachment) {
	if (Zip($attachment['source'], $attachment['destination'])) {
		$attachMentFilePath = $attachment['destination'];
		$file = new MimePart(fopen($attachMentFilePath, 'r'));
		$file->encoding = \Zend\Mime\Mime::ENCODING_BASE64;
		$file->type = finfo_file(finfo_open(), $attachMentFilePath, FILEINFO_MIME_TYPE);
		$file->disposition = \Zend\Mime\Mime::DISPOSITION_ATTACHMENT;
		$file->filename =basename($attachMentFilePath);
		$files[] = $file;
	} else {
		$body .= '<p>Nem sikerult csatolni a ' . $attachment['destination'] . ' cuccot:(<p/>';
	}
}
	
$text = new MimePart('Ez jobb mint a lamafarokszor.');
$text->type = \Zend\Mime\Mime::TYPE_TEXT;

$mimeParts = $files;
$mimeParts[] = $text;

$mailBody = new MimeMessage();
$mailBody->setParts($mimeParts);
$message->setBody($mailBody);
$message->setSubject('Szevasz ecsem!');
$message->setFrom($fromAddress, $fromName);
foreach ($toAdresses as $toAddress) {
	$message->addTo($toAddress['address'], $toAddress['name']);
}


$transport->setOptions($options);

$transport->send($message);

/*
* Fully recursive Zip function
*/

function Zip($source, $destination)
{
	
	if (!extension_loaded('zip') || !file_exists($source)) {
        return FALSE;
    }
	
	$zip = new ZipArchive();
	
	if (!$zip->open($destination, ZIPARCHIVE::CREATE)) {
		return FALSE;
    }
		
	$source = str_replace('\\', '/', realpath($source));
	
	if (is_dir($source)) {
		$files = new RecursiveIteratorIterator(new RecursiveDirectoryIterator($source), RecursiveIteratorIterator::SELF_FIRST);
		
		foreach ($files as $file) {
			$file = str_replace('\\', '/', $file);
		
			// Ignore "." and ".." folders
			if (in_array(substr($file, strrpos($file, '/') + 1), array('.', '..')))
				continue;
		
			$file = realpath($file);
		
			if (is_dir($file) === true) {
				$zip->addEmptyDir(str_replace($source . '/', '', $file . '/'));
			} else if (is_file($file) === true) {
				$zip->addFromString(str_replace($source . '/', '', $file), file_get_contents($file));
			}
		}
	} else {
		$zip->addFromString(basename($source), file_get_contents($source));
	}
	
    return $zip->close();
}


?>