<?php
require('../currentversion.php');

	if(isset($_POST['myversion']))
	{
		$myversion = $_POST['myversion'];
		
		if($myversion == $currentversion)
		{
			header('Versionreply: 0'); 
		}
		else
		{
			header('Versionreply: 1'); 
			header('Versiondownloadlink: ' . $downloadurl); 
		}
	}
	else
	{
		header('Versionreply: 3'); 
	}
?>