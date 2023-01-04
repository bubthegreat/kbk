resource "aws_instance" "kbk_server" {
    ami                                  = "ami-0574da719dca65348"
    associate_public_ip_address          = true
    availability_zone                    = "us-east-1a"
    instance_type                        = "t2.micro"
    key_name                             = "kbk-server"
    subnet_id                            = "subnet-0d2d1d0400bbbc709"
    tags                                 = {
        "Name" = "kbk-server"
    }
    vpc_security_group_ids               = [
        "sg-0d884338507b102c9",
    ]



    root_block_device {
        delete_on_termination = false
        volume_size           = 16
        volume_type           = "gp2"
    }

}

resource "null_resource" "clone_kbk" {
  provisioner "local-exec" {
    command = "cd /home/ubuntu && git clone https://gitlab.com/bubthegreat/kbk.git >> private_ips.txt"
    interpreter = ["bash"]
  }
}





