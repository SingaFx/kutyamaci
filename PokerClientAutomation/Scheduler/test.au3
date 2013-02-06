			Local $name = "Hold'em"
			AutoItSetOption("WinTitleMatchMode", 2)
			$array = WinList($name)
			Local $k
			for $k = 1 to $array[0][0]
			   Local $table = WinGetPos($array[$k][1])
			   MouseClick("left", $table[0] + 13, $table[1] + 330, 1, 1)
			Next