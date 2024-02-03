extends Camera3D


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	var input_dir = Input.get_vector("ui_left", "ui_right", "ui_up", "ui_down")
	var up = Input.get_axis("ui_accept", "ui_cancel")
	position += Vector3(input_dir.x, up, input_dir.y) * 0.5
	
	pass
