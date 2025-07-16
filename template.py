# Prototype CLI + help messages by leveraging click

import click


@click.command()
@click.option("-r", "--rename", is_flag=True, help="Rename files of iOS origin")
@click.argument("file", required=False)
def main(file):
    pass


if __name__ == "__main__":
    main()
